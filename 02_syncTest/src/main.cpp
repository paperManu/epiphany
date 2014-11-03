#include <cstddef>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include <e-hal.h>

#include "common.h"

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

    if (e_load_group((char*)"e_main.srec", &epiphany, 0, 0, platform.rows, platform.cols, E_FALSE) == E_ERR)
    {
        cout << "Error while loading program into Epiphany" << endl;
        exit(1);
    }

    Mailbox mailbox;

    e_alloc(&shm, 0x00000000, sizeof(mailbox));
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            mailbox.sync[i][j] = 0;

    e_write(&shm, 0, 0, 0x0, &mailbox, sizeof(Mailbox));

    cout << "Launching..." << endl;
    e_start_group(&epiphany);

    bool allSynced = false;
    while (!allSynced)
    {
        e_read(&shm, 0, 0, offsetof(Mailbox, sync[0][0]), &(mailbox.sync), 16 * sizeof(int));
        allSynced = true;
        int count = 0;
        cout << "syncing..." << endl;
        for (unsigned int i = 0; i < 4; ++i)
            for (unsigned int j = 0; j < 4; ++j)
            {
                cout << " --> " << i << " x " << j << " - " << mailbox.sync[i][j] << endl;
                if (mailbox.sync[i][j] == 0)
                    allSynced = false;
                else
                    count++;
            }
        cout << " - count = " << count << endl;
    }

    e_free(&shm);
    e_finalize();

    return 0;
}
