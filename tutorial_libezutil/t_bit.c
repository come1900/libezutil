/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * t_bit.c - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: t_bit.c 5884 2013-02-06 03:07:00Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-02-06 03:07:00  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <stdlib.h>

#include <ezutil/ez_bit.h>

//#define BitGet(Number,pos) ((Number) >> (pos)&1) //：??o：o|━?|━??3：oy|━??3??
//#define BitSet(Number,pos) ((Number) | 1<<(pos)) //????3????1
//#define BitClear(Number,pos) ((Number) & ~(1<<(pos))) //????3????0
//#define BitAnti(Number,pos) ((Number) ^ 1<<(pos)) //???Number|━?POS??：：??∴??

void dump_mask(unsigned int my_mask);
int main(int argc, char *argv[])
{
    unsigned int my_mask = 0;


    printf("test BitGet, show(0x%0x):\n", my_mask);
    dump_mask(my_mask);

    my_mask = BitSet(my_mask, 1);
    printf("test BitSet, BitSet(my_mask, 0) then show(0x%0x):\n", my_mask);
    dump_mask(my_mask);
    my_mask = BitSet(my_mask, 11);
    printf("test BitSet, BitSet(my_mask, 0) then show(0x%0x):\n", my_mask);
    dump_mask(my_mask);

    my_mask = BitClear(my_mask, 1);
    printf("test BitSet, BitClear(my_mask, 0) then show(0x%0x):\n", my_mask);
    dump_mask(my_mask);

    my_mask = BitAnti(my_mask, 1);
    printf("test BitSet, BitAnti(my_mask, 0) then show(0x%0x):\n", my_mask);
    dump_mask(my_mask);

    my_mask = BitAnti(my_mask, 1);
    printf("test BitSet, BitAnti(my_mask, 0) then show(0x%0x):\n", my_mask);
    dump_mask(my_mask);

    return 0;
}

void dump_mask(unsigned int my_mask)
{
    int i=0;
    
    for (i=0; i<sizeof(my_mask)*8; i++)
    {
        printf("%d ", BitGet(my_mask, i));
    }
    
    printf("\n");
}
