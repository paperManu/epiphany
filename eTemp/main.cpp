#include <iostream>
#include <fstream>
#include <signal.h>
#include <string>
#include <thread>

using namespace std;

#define RAW_TEMP_FILE "/sys/bus/iio/devices/iio:device0/in_temp0_raw"
#define SCALE_TEMP_FILE "/sys/bus/iio/devices/iio:device0/in_temp0_scale"
#define OFFSET_TEMP_FILE "/sys/bus/iio/devices/iio:device0/in_temp0_offset"

/*************/
float readFile(string filename)
{
    ifstream file(filename, ios::in | ios::binary);
    string content;
    if (file)
    {
        file.seekg(0, ios::end);
        content.resize(file.tellg());
        file.seekg(0, ios::beg);
        file.read(&content[0], content.size());
        file.close();
    }
    return stof(content);
}

bool run;

/*************/
void leave(int signal_value)
{
    run = false;
    cout << endl;
}

/*************/
int main(int argc, char** argv)
{
    struct sigaction signals;
    signals.sa_handler = leave;
    signals.sa_flags = 0;
    sigaction(SIGINT, &signals, NULL);
    sigaction(SIGTERM, &signals, NULL);

    cout << " -- Epiphany temperature -- " << endl;

    float offset = readFile(OFFSET_TEMP_FILE);
    float scale = readFile(SCALE_TEMP_FILE);
    
    run = true;
    while (run)
    {
        float rawValue = readFile(RAW_TEMP_FILE);
        float temp = (rawValue + offset) * scale / 1000.f;

        cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << " --> " << temp << "°C";
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }
}
