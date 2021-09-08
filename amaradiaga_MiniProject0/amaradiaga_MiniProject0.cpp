#include "amaradiaga_binaryutils.h";

void main()
{
    /* i */
    uint32_t solo = 0;
    setbit(&solo, 24);
    //printf("setbit for 24:\n\r");
    //display_binary(solo);

    /* ii */
    setbit(&solo, 16);
    setbit(&solo, 17);
    //printf("setbit for 16 and 17:\n\r");
    //display_binary(solo);

    /* iii */
    setbits(&solo, 4095);
    //printf("setbits forrun 0 - 11 (4095):\n\r");
    //display_binary(solo);

    /* iv */
    clearbit(&solo, 11);
    //printf("clearbit for 11:\n\r");
    //display_binary(solo);

    /* v */
    clearbits(&solo, 240);
    //printf("clearbits for 4 - 7 (240):\n\r");
    //display_binary(solo);

    //printf("Solo: %d\n", solo);

    printf("binary solo:\n\r");
    display_binary(solo);

}