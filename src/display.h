#ifndef DISPLAY_H
#define DISPLAY_H

#include "global.h"


//-------------
// Functions
//-------------

void init_display();

void opcode_to_string(uint64_t f_instruction, char* operation);

char mode_to_char(instruction_mode f_mode);

void display_instruction(uint64_t f_instruction, int f_positionY);

void display_core(core_cell* f_core);

void display_bot(bot* f_bot, int f_positionY);

void display_score_bot(int f_score1, int f_score2);

void display_winner(int f_id_bot);

#endif
