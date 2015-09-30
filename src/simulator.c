#include "simulator.h"


/*
 * Simulate the bot, execute his current instruction
 */
void simulate_bot(bot* f_bot, core_cell* f_core)
{
    if (f_bot->current_process->next_process != NULL)
    {
        f_bot->current_process = f_bot->current_process->next_process;
    }
    else
    {
        f_bot->current_process = f_bot->list_process;
    }

    process* _process = f_bot->current_process;

    f_core[_process->position].modifier = f_bot->id;

    uint64_t _instruction = f_core[_process->position].instruction;

    save_instruction(_process, _instruction);

    switch (get_opcode(_instruction))
    {
        case DAT:
            _process->abort = true;
            break;

        case MOV:
            {
                int _addressB = get_addressB(_process, f_core, _instruction, &_process->abort);

                if (get_modeA(_instruction) == IMMEDIATE)
                {
                    f_core[_addressB].instruction = get_fieldA(_instruction);
                }
                else
                {
                    int _addressA = get_addressA(_process, f_core, _instruction, &_process->abort);

                    f_core[_addressB].instruction = f_core[_addressA].instruction;
                }
                f_core[_addressB].modifier = f_bot->id;
            }
            break;

        case ADD:
            {
                int _addressB = get_addressB(_process, f_core, _instruction, &_process->abort);
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);
                int _operandA = get_operandA(_process, f_core, _instruction, &_process->abort);

                _process->answer_instruction = _operandA + _operandB;

                push_data(_addressB, f_bot->id, f_core, _process->answer_instruction);
            }
            break;

        case SUB:
            {
                int _addressB = get_addressB(_process, f_core, _instruction, &_process->abort);
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);
                int _operandA = get_operandA(_process, f_core, _instruction, &_process->abort);

                _process->answer_instruction = _operandB - _operandA;

                push_data(_addressB, f_bot->id, f_core, _process->answer_instruction);
            }
            break;

        case JMP:
            {
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);

                _process->position = normalize_address(_process->position + _operandB - 1);
            }
            break;

        case JMZ:
            {
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);
                int _operandA = get_operandA(_process, f_core, _instruction, &_process->abort);

                if (_operandA == 0)
                {
                    _process->position = normalize_address(_process->position + _operandB - 1);
                }
            }
            break;

        case DJZ:
            {
                int _addressA = get_addressA(_process, f_core, _instruction, &_process->abort);
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);
                int _operandA = get_operandA(_process, f_core, _instruction, &_process->abort);

                _process->answer_instruction = _operandA - 1;

                push_data(_addressA, f_bot->id, f_core, _process->answer_instruction);

                if (_process->answer_instruction == 0)
                {
                    _process->position = normalize_address(_process->position + _operandB - 1);
                }
            }
            break;

        case CMP:
            {
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);
                int _operandA = get_operandA(_process, f_core, _instruction, &_process->abort);

                if (_operandA == _operandB)
                {
                    _process->position = normalize_address(_process->position + 1);
                }
            }
            break;

        case SPL:
            {
                int _operandB = get_operandB(_process, f_core, _instruction, &_process->abort);

                // New process
                add_process(f_bot, _process->position + _operandB);
            }
            break;

    }

    // next instruction
    _process->position = normalize_address(_process->position + 1);


    // if the process has aborted
    if (_process->abort == true)
    {
        // first element
        if (_process == f_bot->list_process)
        {
            if (_process->next_process != NULL)
            {
                // list process is empty
                f_bot->list_process = NULL;
            }
            else
            {
                f_bot->list_process = _process->next_process;
            }
        }
        else
        {
            // search previous element and relink
            for (process* _p = f_bot->list_process; _p != NULL; _p = _p->next_process)
            {
                if (_p->next_process == _process)
                {
                    _p->next_process = _process->next_process;
                }
            }
        }
    }
}


/*
 * Save 5 last executed instructions
 */
void save_instruction(process* f_process, uint64_t f_instruction)
{
    for (int i = 4; i != 0; i--)
    {
        f_process->last_instruction[i] = f_process->last_instruction[i - 1];
    }

    f_process->last_instruction[0] = f_instruction;
}


/*
 * Add a new process to the list of process
 */
void add_process(bot* f_bot, int f_position)
{
    process* _process = malloc(sizeof(process));
    if (_process == NULL)
    {
        return;
    }

    _process->next_process = f_bot->current_process->next_process;
    f_bot->current_process->next_process = _process;

    _process->abort = false;
    _process->position = f_position;
    _process->answer_instruction = 0;

    for (int i = 0; i < 5; i++)
    {
        _process->last_instruction[i] = 0;
    }

}


/*
 * Get operation code from instruction
 *
 * Return : instruction's opcode
 */
instruction_opcode get_opcode(uint64_t f_instruction)
{
    return (instruction_opcode)(f_instruction >> 28);
}


/*
 * Get mode A from instruction
 *
 * Return : mode A
 */
instruction_mode get_modeA(uint64_t f_instruction)
{
    return (instruction_mode)((f_instruction >> 26) % 4);
}


/*
 * Get mode B from instruction
 *
 * Return : mode B
 */
instruction_mode get_modeB(uint64_t f_instruction)
{
    return (instruction_mode)((f_instruction >> 24) % 4);
}


/*
 * Get field A from instruction
 *
 * Return : value of A
 */
int get_fieldA(uint64_t f_instruction)
{
    int _binary_value = ((f_instruction >> 12) & ((1 << 12) - 1));

    // if the 11th bit is 1, the number will be negative
    if ((_binary_value & (1 << 11)) == 0)
    {
        return _binary_value;
    }
    else
    {
        return -(_binary_value & ((1 << 11) - 1));
    }
}

/*
 * Get field B from instruction
 *
 * Return : value of B
 */
int get_fieldB(uint64_t f_instruction)
{
    int _binary_value = (f_instruction & ((1 << 12) - 1));

    // if the 11th bit is 1, the number will be negative
    if ((_binary_value & 2048) == 0)
    {
        return _binary_value;
    }
    else
    {
        return -(_binary_value & ((1 << 11) - 1));
    }
}

/*
 * Compute the address
 * Set f_abort to true if f_mode is IMMEDIATE or if an error occur
 *
 * Return : adress of field A
 */
int get_addressA(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort)
{
    switch (get_modeA(f_instruction))
    {
        case IMMEDIATE:
            {
                *f_abort = true;
                return 0;
            }

        case RELATIVE:
            return normalize_address(f_process->position + get_fieldA(f_instruction));

        case INDIRECT:
            {
                int _pointer = normalize_address(f_process->position + get_fieldA(f_instruction));
                return normalize_address(_pointer + get_fieldB(f_core[_pointer].instruction));
            }

        default:
            {
                *f_abort = true;
                return 0;
            }
    }

}


/*
 * Compute the address
 * Set f_abort to true if f_mode is IMMEDIATE or if an error occur
 *
 * Return : adress of field B
 */
int get_addressB(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort)
{
    switch (get_modeB(f_instruction))
    {
        case IMMEDIATE:
            {
                *f_abort = true;
                return 0;
            }

        case RELATIVE:
            return (f_process->position + get_fieldB(f_instruction)) % SIZE_CORE;

        case INDIRECT:
            {
                int _pointer = normalize_address(f_process->position + get_fieldB(f_instruction));
                return normalize_address(_pointer + get_fieldB(f_core[_pointer].instruction));
            }

        default:
            {
                *f_abort = true;
                return 0;
            }
    }

}


/*
 * Normalize the address to be sure it is between 0 and SIZE_CORE
 *
 * Return : adress normalized
 */
int normalize_address(int f_address)
{
    if (f_address < 0)
    {
        f_address += SIZE_CORE;
    }

    return f_address % SIZE_CORE;
}


/*
 * Compute the operand of the field A
 *
 * Return : operand A
 */
int get_operandA(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort)
{
    switch (get_modeA(f_instruction))
    {
        case IMMEDIATE:
            return get_fieldA(f_instruction);

        case RELATIVE:
        case INDIRECT:
            {
                int _address = get_addressA(f_process, f_core, f_instruction, f_abort);
                return get_fieldB(f_core[_address].instruction);
            }

        default:
            {
                *f_abort = true;
                return 0;
            }
    }
}


/*
 * Compute the operand of the field B
 *
 * Return : operand B
 */
int get_operandB(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort)
{
    switch (get_modeB(f_instruction))
    {
        case IMMEDIATE:
            return get_fieldB(f_instruction);

        case RELATIVE:
        case INDIRECT:
            {
                int _address = get_addressB(f_process, f_core, f_instruction, f_abort);
                return get_fieldB(f_core[_address].instruction);
            }

        default:
            {
                *f_abort = true;
                return 0;
            }
    }
}

/*
 * Convert the instruction to a DAT
 */
void push_data(int f_address, int f_id_bot, core_cell* f_core, int f_value)
{
    if (f_value >= 0)
    {
        f_core[f_address].instruction = f_value;
    }
    else
    {
        f_core[f_address].instruction = -f_value + (1 << 11);
    }

    f_core[f_address].modifier = f_id_bot;
}
