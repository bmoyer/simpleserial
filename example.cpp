#include "SimpleSerial.h"

int main()
{
    SimpleSerial p;
    p.open("/dev/ttyACM0", 115200);
    usleep(1000000);

    p.flush_both_buffers();
    for(int i = 0; i < 20; i++)
    {
        p.write_line("foo " + std::to_string(i*10000));
        std::cout << p.read_line();
    }

    p.close();

    return 0;
}

