#include "amaradiaga_binaryutils.h"

void setbit(uint32_t* addr, uint8_t whichbit)
{
	uint32_t new_addr = 1;  //function should only set bit at position whichbit to 1
	new_addr = new_addr << whichbit;
	*addr = *addr | new_addr;
}

void clearbit(uint32_t* addr, uint8_t whichbit)
{
	uint32_t new_addr = 1;
	new_addr = new_addr << whichbit;
	new_addr = ~new_addr; //not operator for new_addr
	*addr = *addr & new_addr;
}

void setbits(uint32_t* addr, uint32_t bitmask)
{
	*addr = *addr | bitmask; //only set bits defined in the mask
}

void clearbits(uint32_t* addr, uint32_t bitmask)
{
	bitmask = ~bitmask;
	*addr = *addr & bitmask; //only clearbits defined in mask
}

void display_binary(uint32_t num)
{
	uint32_t num1 = num;
	int tmp = 0;
	char binaryrep[128]; //it should be 128 because char will take 4 bits of memory. 32*4=128
	for (unsigned i = (1 << 31); i > 0; i = i / 2) { //dividing i by 2 is equivalent to shifting i by 1 bit to the right. Compare 1 to index number and if they're both 1 then include in variable (binaryrep)
		binaryrep[tmp++] = (num & i) ? '1' : '0'; //if the ith bit is > 0, then it's 1 if not it's to 0
		binaryrep[tmp] = '\0';
		printf("%s\n", binaryrep); //display to console the binary representation
	}
}
