# simpleserial

simpleserial is a light, cross-platform wrapper around boost's `asio::serial_port` class. This
project is designed to offer a Python-like interface to serial communication in C++.

Example:
```
#include "SimpleSerial.h"

using namespace std;

int main()
{
    SimpleSerial serial;
    serial.open("/dev/ttyACM0", 115200);

    if(serial.is_open())
    {
        serial.write_line("Hello, world!");
        string data = serial.read_line();
    }

    serial.close();

    return 0;
}
```

