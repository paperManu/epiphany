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
    e_mem_t emem;

    e_init(NULL);
    e_reset_system();
    e_get_platform_info(&platform);
    //e_set_loader_verbosity(L_D1);

    e_alloc(&emem, 0x01000000, 128);

    mutex eMutex;
    vector<thread> threads;
    for (unsigned int row = 0; row < 4; ++row)
        for (unsigned int col = 0; col < 4; ++col)
        {
            threads.emplace_back([=, &emem, &platform, &eMutex] {
                unsigned int coreid = (row + platform.row) * 64 + col + platform.col;

                e_epiphany_t dev;
                e_open(&dev, row, col, 1, 1);
                e_reset_group(&dev);

                eMutex.lock();
                e_load("e_main.srec", &dev, 0, 0, E_FALSE);
                eMutex.unlock();

                eMutex.lock();
                e_start(&dev, 0, 0);
                this_thread::sleep_for(chrono::milliseconds(10));
                char emsg[128];
                e_read(&emem, 0, 0, 0x0, emsg, 128);
                cout << "Message from eCore (" << row << "," << col << ") : ";
                cout << "\"" << emsg << "\"" << endl;
                eMutex.unlock();

                e_close(&dev);
            });
        }

    for (auto& t : threads)
        t.join();

    e_free(&emem);
    e_finalize();

    return 0;
}
