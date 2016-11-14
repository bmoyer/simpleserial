#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp> 
#include <memory>
#include <iostream>
#include <stdio.h>
#include <string>

enum Parity { 
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_NONE
};

enum StopBits {
    STOPBITS_ONE,
    STOPBITS_ONE_POINT_FIVE,
    STOPBITS_TWO
};

enum FlowControl {
    FLOW_CONTROL_NONE,
    FLOW_CONTROL_SOFTWARE,
    FLOW_CONTROL_HARDWARE
};

class SimpleSerial {

public:
    void open(std::string portName, long baud = 9600, int byteSize = 8, 
            Parity parity = PARITY_NONE, StopBits stopBits = STOPBITS_ONE,
            FlowControl flowControl = FLOW_CONTROL_NONE);
    void close();
    bool is_open();

    // Write methods
    int write(std::string data);
    int write_line(std::string data);

    // Read methods
    std::string read(int numBytes);
    std::string read_line();

    // Buffer flushing
    void flush_receive_buffer();
    void flush_send_buffer();
    void flush_both_buffers();

    // Serial communication settings
    void set_flow_control(FlowControl flowControl);
    void set_stop_bits(StopBits stopBits);
    void set_parity(Parity parity);

private:
    void flush(int flushBuffer);

    boost::asio::io_service io;
    std::unique_ptr<boost::asio::serial_port> port;
};

