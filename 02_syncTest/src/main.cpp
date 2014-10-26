#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <e-hal.h>

using namespace std;

/*************/
int main(int argc, char** argv)
{
    e_platform_t platform;
    e_epiphany_t epiphany;
    e_mem_t shm;

    e_set_host_verbosity(H_D0);
    e_set_loader_verbosity(L_D0);
    e_init(NULL);
    e_reset_system();
    e_get_platform_info(&platform);

    if (e_open(&epiphany, 0, 0, platform.rows, platform.cols))
    {
        cout << "Error while establishing connection to the Epiphany" << endl;
        exit(1);
    }

    if (e_load_group((char*)"e_main.srec", &epiphany, 0, 0, platform.rows, platform.cols, E_TRUE) == E_ERR)
    {
        cout << "Error while loading program into Epiphany" << endl;
        exit(1);
    }

    unsigned int shmSize = 0x00400000;
    e_alloc(&shm, 0x00000000, shmSize);

    e_free(&shm);
    e_finalize();

    return 0;
}
