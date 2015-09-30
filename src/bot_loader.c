#include "bot_loader.h"
#include <string.h>


/*
 * Convert instruction
 *
 * f_instruction : instruction to convert
 * Return : instruction converted in uint64_t
 */
uint64_t instruction_convert(char* f_instruction)
{
    char _symbol[4];

    // Get instruction's type (MOV, DAT, JMP, ...)
    for (int i = 0; i < 3; i++)
    {
        _symbol[i] = f_instruction[i];
    }
    _symbol[3] = 0;

    uint64_t _symbol_encoded = encoded_symbol(_symbol);

    char _mode_A;
    char _mode_B;
    uint64_t _encoded_mode_A;
    uint64_t _encoded_mode_B;
    uint64_t _A;
    uint64_t _B;
    int _end = 0 ;

    // Instructions DAT,JMP AND SPL
    if ((_symbol_encoded == DAT) || (_symbol_encoded == JMP) || (_symbol_encoded == SPL))
    {
        _symbol_encoded = _symbol_encoded << 28 ;

        _B = get_mode_and_value(4, f_instruction, &_mode_B, &_end);
        _encoded_mode_B = encoded_mode(_mode_B);
        _encoded_mode_B = _encoded_mode_B << 24;

        return _symbol_encoded + _encoded_mode_B + _B ;
    }

    // Others instructions
    else
    {
        _symbol_encoded = _symbol_encoded << 28 ;

        _A = get_mode_and_value(4, f_instruction, &_mode_A, &_end);
        _B = get_mode_and_value(_end, f_instruction, &_mode_B, &_end);

        _encoded_mode_A = encoded_mode(_mode_A);
        _encoded_mode_B = encoded_mode(_mode_B);

        _encoded_mode_A = _encoded_mode_A << 26;
        _encoded_mode_B = _encoded_mode_B << 24;

        _A = _A << 12;

        return (_symbol_encoded + _encoded_mode_A + _encoded_mode_B + _A + _B) ;

    }
}


/*
 * Get mode and value for the instruction
 *
 * f_begin : start index for the loop
 * f_instruction : instruction to analyze
 * f_mode : Get mode's value
 * f_end : Get ending of loop value
 *
 * Return : integer value of argument
 */
uint64_t get_mode_and_value(int f_begin, char* f_instruction, char* f_mode, int* f_end)
{
    char _argument[20];
    int _negatif = 0;
    int k = 0;
    int j;

    *f_mode = ' ';

    for (j = f_begin; f_instruction[j] != ' ' && f_instruction[j] != '\0'; j++)
    {
        // a number is read
        if (f_instruction[j] >= '0' && f_instruction[j] <= '9')
        {
            _argument[k] = f_instruction[j];
            k++;
        }
	// a minus is read: number is negative
        else if (f_instruction[j] == '-')
        {
            _negatif = 1;
        }
	// "#" read: mode is IMMEDIATE
        else if (f_instruction[j] == '#')
        {
            *f_mode = '#';
        }
	// "@" read: mode is INDIRECT
        else if (f_instruction[j] == '@')
        {
            *f_mode = '@';
        }
    }
    _argument[k] = '\0';
    *f_end = j + 1;

    if (_negatif == 0)
    {
        return atoi(_argument);
    }
    else
    {
        return atoi(_argument) + (1 << 11);
    }
}


/*
 * Encode instruction's mode
 *
 * f_mode : mode instruction
 * Return : mode encoded
 */
uint64_t encoded_mode(char f_mode)
{
    if (f_mode == '#')
    {
        return IMMEDIATE;
    }
    else if (f_mode == ' ')
    {
        return RELATIVE;
    }
    else if (f_mode == '@')
    {
        return INDIRECT;
    }
    else
    {
        printf("Syntax error\n");
        exit(EXIT_FAILURE);
    }

}

/*
 * Encode instruction's symbol
 *
 * f_symbol : operation code of instruction
 * Return : opcode encoded
 */
uint64_t encoded_symbol(char* f_symbol)
{
    if (strcmp(f_symbol, "DAT") == 0)
    {
        return 0;
    }
    else if (strcmp(f_symbol, "MOV") == 0)
    {
        return 1;
    }
    else if (strcmp(f_symbol, "ADD") == 0)
    {
        return 2;
    }
    else if (strcmp(f_symbol, "SUB") == 0)
    {
        return 3;
    }
    else if (strcmp(f_symbol, "JMP") == 0)
    {
        return 4;
    }
    else if (strcmp(f_symbol, "JMZ") == 0)
    {
        return 5;
    }
    else if (strcmp(f_symbol, "DJZ") == 0)
    {
        return 6;
    }
    else if (strcmp(f_symbol, "CMP") == 0)
    {
        return 7;
    }
    else if (strcmp(f_symbol, "SPL") == 0)
    {
        return 8;
    }

    else
    {
        printf("Syntax error\n");
        exit(EXIT_FAILURE);
    }
}


/*
 * Load bot from file : convert all instructions and place bot in core
 */
void load_bot(char* f_name, bot* f_bot, core_cell* f_core)
{
    int _size_max = 100;
    FILE* _bot;
    char _instruction[_size_max];
    uint64_t _array_instructions[_size_max];
    int i = 0;

    int _position ;

    _bot = fopen(f_name, "r");

    // Convert instructions
    if (_bot != NULL)
    {
        while (fgets(_instruction, _size_max, _bot) != NULL)
        {
            _array_instructions[i] = instruction_convert(_instruction);
            i++;
        }

        fclose(_bot);
    }

    // Place bot in core
    int done = 0;
    while (done == 0)
    {
        _position = rand() % SIZE_CORE;
        done = 1;
        for (int j = 0; j < i; j++)
        {
            if (f_core[(_position + j) % SIZE_CORE].modifier != 0)
            {
                done = 0;
            }
        }
    }

    f_bot->list_process->position = _position;

    for (int k = 0; k < i; k++)
    {
        f_core[(k + _position) % SIZE_CORE].instruction = _array_instructions[k];
        f_core[(k + _position) % SIZE_CORE].modifier = f_bot->id ;
    }
}


/*
 * Initialize bot
 */
void initialize_bot(bot* f_bot, int id)
{
    f_bot->list_process = malloc(sizeof(process));

    if (f_bot->list_process == NULL)
    {
        exit(EXIT_FAILURE);
    }

    f_bot->list_process->next_process = NULL;
    f_bot->current_process = f_bot->list_process;

    f_bot->id = id;
    f_bot->list_process->abort = false;
    f_bot->list_process->answer_instruction = 0;

    for (int i = 0; i < 5; i++)
    {
        f_bot->list_process->last_instruction[i] = 0;
    }

}

