#pragma once
#include <stdint.h> 
#include <stdio.h>

#ifndef BITVARIABLE
#define BITVARIABLE

/*
- Author: Anya Maradiaga
- Date: Tuesday August 8, 2021
- Description:
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

