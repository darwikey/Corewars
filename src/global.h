#ifndef GLOBAL_H
#define GLOBAL_H


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdint.h>



//------------------
// Define
//------------------
#define SIZE_CORE 4096
#define SCREEN_WIDTH 150


//------------------
// Enumerate
//------------------
typedef enum
{
    DAT,
    MOV,
    ADD,
    SUB,
    JMP,
    JMZ,
    DJZ,
    CMP,
    SPL

} instruction_opcode;


typedef enum
{
    IMMEDIATE,
    RELATIVE,
    INDIRECT

} instruction_mode;



//------------------
// Structure
//------------------

typedef struct
{
    // instruction
    uint64_t instruction;

    // id of the bot who have modified the instruction
    int modifier;

} core_cell;


typedef struct process
{
    // if the bot execute a wrong instruction
    bool abort;

    // position of the current instruction
    int position;

    // some instructions have to store their value
    int answer_instruction;

    // latest instructions
    uint64_t last_instruction[5];

    struct process* next_process;

} process;


typedef struct
{
    // 1 or 2
    int id;

    process* list_process;

    process* current_process;
} bot;





#endif
