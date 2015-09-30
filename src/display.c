#include "display.h"
#include "simulator.h"
#include <string.h>


/*
 * Init window and colors used in window
 */
void init_display()
{
    // Start curses mode
    initscr();

    // Init colors
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);

    // Window background and foreground
    bkgd(COLOR_PAIR(1));

}


/*
 * Convert instruction's operation code to string
 */
void opcode_to_string(uint64_t f_instruction, char* f_operation)
{
    instruction_opcode _opcode = get_opcode(f_instruction);
    switch (_opcode)
    {
        case DAT:
            strcpy(f_operation, "DAT") ;
            break;
        case MOV:
            strcpy(f_operation, "MOV");
            break;
        case ADD:
            strcpy(f_operation, "ADD");
            break;
        case SUB:
            strcpy(f_operation, "SUB");
            break;
        case JMP:
            strcpy(f_operation, "JMP");
            break;
        case JMZ:
            strcpy(f_operation, "JMZ");
            break;
        case DJZ:
            strcpy(f_operation, "DJZ");
            break;
        case CMP:
            strcpy(f_operation, "CMP");
            break;
        case SPL:
            strcpy(f_operation, "SPL");
            break;

    }
}


/*
 * Convert instruction's mode to char
 *
 * Return : char corresponding to f_mode
 */
char mode_to_char(instruction_mode f_mode)
{
    switch (f_mode)
    {
        case IMMEDIATE:
            return '#' ;
        default:
        case RELATIVE:
            return ' ';
        case INDIRECT:
            return '@';

    }
}

/*
 * Display an instruction at specified position Y
 */
void display_instruction(uint64_t f_instruction, int f_positionY)
{
    char _operation[4];
    opcode_to_string(f_instruction, _operation);

    char _modeA = mode_to_char(get_modeA(f_instruction));
    char _modeB = mode_to_char(get_modeB(f_instruction));

    if ((get_opcode(f_instruction) == DAT) || (get_opcode(f_instruction) == JMP) || (get_opcode(f_instruction) == SPL))
    {
        mvprintw(f_positionY, SCREEN_WIDTH - 30, "%s %c%d", _operation, _modeB, get_fieldB(f_instruction));
    }
    else
    {
        mvprintw(f_positionY, SCREEN_WIDTH - 30, "%s %c%d %c%d", _operation, _modeA, get_fieldA(f_instruction), _modeB, get_fieldB(f_instruction));
    }

}


/*
 * display core
 */
void display_core(core_cell* f_core)
{
    const int _width_table = SCREEN_WIDTH - 40;

    for (int i = 0; i < SIZE_CORE; i++)
    {
        if (f_core[i].modifier == 0)
        {
            mvprintw(i / _width_table, i % _width_table, "%d", 0);
        }
        else if (f_core[i].modifier == 1)
        {
            attron(COLOR_PAIR(2));
            mvprintw(i / _width_table, i % _width_table, "%d", 1);
            attroff(COLOR_PAIR(2));
        }
        else if (f_core[i].modifier == 2)
        {
            attron(COLOR_PAIR(3));
            mvprintw(i / _width_table, i % _width_table, "%d", 2);
            attroff(COLOR_PAIR(3));
        }
    }
}

/*
 * Display bot : number and instructions
 */
void display_bot(bot* f_bot, int f_positionY)
{
    // Display bot's number
    attron(COLOR_PAIR(4));
    mvprintw(f_positionY, SCREEN_WIDTH - 33, "Robot %d ", f_bot->id);
    attroff(COLOR_PAIR(4));

    if (f_bot->current_process == NULL)
    {
        return;
    }

    // display 5 last instructions of the current process
    for (int j = 0; j < 5; j++)
    {
        if (f_bot->current_process->last_instruction[j] != 0)
        {
            display_instruction(f_bot->current_process->last_instruction[j], f_positionY + j + 1);
        }
    }

    // Bot is crashed
    if (f_bot->current_process->abort)
    {
        attron(COLOR_PAIR(4));
        mvprintw(f_positionY, SCREEN_WIDTH - 33, "Le robot %d a crashé ! ", f_bot->id);
        attroff(COLOR_PAIR(4));
    }
}


/*
 * Display bot scores and tour
 */
void display_score_bot(int f_score1, int f_score2)
{
    attron(COLOR_PAIR(5));
    mvprintw(20, SCREEN_WIDTH - 33, "Tour %d", f_score1 + f_score2);
    mvprintw(21, SCREEN_WIDTH - 30, "Score robot 1 : %d", f_score1);
    mvprintw(22, SCREEN_WIDTH - 30, "Score robot 2 : %d", f_score2);
    attroff(COLOR_PAIR(5));
}


void display_winner(int f_id_bot)
{
    attron(COLOR_PAIR(6));
    mvprintw(26, SCREEN_WIDTH - 33, "Le gagnant est le robot %d", f_id_bot);
    attroff(COLOR_PAIR(6));
}
