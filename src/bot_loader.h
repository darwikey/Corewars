#ifndef BOT_LOADER
#define BOT_LOADER


#include "global.h"


//--------------
// Functions
//--------------

uint64_t instruction_convert(char* instruction);

uint64_t encoded_symbol(char* f_symbol);

uint64_t encoded_mode(char f_mode);

uint64_t get_mode_and_value(int f_begin, char* f_instruction, char* f_mode, int* f_end);

void load_bot(char* f_name, bot* f_bot, core_cell* f_core);

void initialize_bot(bot* f_bot, int id);


#endif
