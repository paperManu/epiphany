#include <stdio.h>
#include <unistd.h>
#include <e_lib.h>

#include "common.h"

//volatile Mailbox mailbox SECTION("shared_dram");
volatile Mailbox* mailbox = (Mailbox*)e_emem_config.base;

/*************/
int main(void)
{
    e_coreid_t coreid = e_get_coreid();
    unsigned int row, col;
    e_coords_from_coreid(coreid, &row, &col);

    //volatile Mailbox* mailbox = (Mailbox*)e_emem_config.base;
    mailbox->sync[row][col] = 1; //(int)(row * 10 + col);

    return 0;
}
