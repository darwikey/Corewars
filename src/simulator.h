#ifndef SIMULATOR_H
#define SIMULATOR_H


#include "global.h"


//-------------
// Functions
//-------------

void simulate_bot(bot* f_bot, core_cell* f_core);

void save_instruction(process* f_process, uint64_t f_instruction);

void add_process(bot* f_bot, int position);

instruction_opcode get_opcode(uint64_t f_instruction);

instruction_mode get_modeA(uint64_t f_instruction);

instruction_mode get_modeB(uint64_t f_instruction);

int get_fieldA(uint64_t f_instruction);

int get_fieldB(uint64_t f_instruction);

int get_addressA(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort);

int get_addressB(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort);

int get_operandA(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort);

int get_operandB(process* f_process, core_cell* f_core, uint64_t f_instruction, bool* f_abort);

int normalize_address(int f_adress);

void push_data(int f_address, int f_id_bot, core_cell* f_core, int f_value);


#endif
