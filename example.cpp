#include "SimpleSerial.h"

int main()
{
    SimpleSerial serial;
    serial.open("/dev/ttyACM0", 115200);
    usleep(1000000);

    serial.flush_both_buffers();
    for(int i = 0; i < 20; i++)
    {
        serial.write_line("xfoo " + std::to_string(i*10000));
        std::cout << serial.read_line();
    }

    serial.close();

    return 0;
}

