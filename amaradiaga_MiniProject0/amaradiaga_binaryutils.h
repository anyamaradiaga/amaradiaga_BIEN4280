#pragma once
#include <stdint.h> 
#include <stdio.h>

#ifndef BITVARIABLE
#define BITVARIABLE

/*
- Author: Anya Maradiaga
- Date: Tuesday August 31, 2021
- Description: The following project shows how bitwise operations like clear or set bit are used. 
*/

//setbit function
void setbit(uint32_t* addr, uint8_t whichbit);

//clearbit function
void clearbit(uint32_t* addr, uint8_t whichbit);

//setbits function
void setbits(uint32_t* addr, uint32_t bitmask);

//clearbits function
void clearbits(uint32_t* addr, uint32_t bitmask);

//display binary function
void display_binary(uint32_t num);

#endif

