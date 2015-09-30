#ifndef EXAMPLE_H
#define EXAMPLE_H


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdint.h>



//------------------
// Define
//------------------
#define SIZE_SCREEN 4096



//------------------
// Enumerate
//------------------

typedef enum
{
  ALPHA,
  BETA,
  GAMMA

} abc;



//------------------
// Structure
//------------------

typedef struct 
{
  char* string;
  int value;

} thing;




#endif
