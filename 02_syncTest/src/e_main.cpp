#include <stdio.h>
#include <e_lib.h>

char outbuf[128] SECTION("shared_dram");

/*************/
int main(void)
{
    e_coreid_t coreid;
    coreid = e_get_coreid();
    sprintf(outbuf, "Hi there, from 0x%03x!", coreid);
    return 0;
}
