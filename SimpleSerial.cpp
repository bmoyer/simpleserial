#include "SimpleSerial.h"

using namespace boost;

void SimpleSerial::open(std::string portName, long baud, int byteSize, 
        Parity parity, StopBits stopBits,
        FlowControl flowControl)
{
    port = std::unique_ptr<asio::serial_port>(new asio::serial_port(io));
    port->open(portName);

    // Set all serial port properties
    port->set_option(asio::serial_port_base::baud_rate(baud));
    port->set_option(asio::serial_port_base::character_size(byteSize));
    set_parity(parity);
    set_stop_bits(stopBits);
    set_flow_control(flowControl);
}

bool SimpleSerial::is_open()
{
    return port->is_open();
}

void SimpleSerial::set_flow_control(FlowControl flowControl)
{
    asio::serial_port_base::flow_control::type flowControlType;
    switch(flowControl)
    {
        case FLOW_CONTROL_NONE:
            flowControlType = asio::serial_port_base::flow_control::none;
            break;
        case FLOW_CONTROL_SOFTWARE:
            flowControlType = asio::serial_port_base::flow_control::software;
            break;
        case FLOW_CONTROL_HARDWARE:
            flowControlType = asio::serial_port_base::flow_control::hardware;
            break;
        default:
            assert(0); // invalid flow control type!
            break;
    }
    port->set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
}

void SimpleSerial::set_stop_bits(StopBits stopBits)
{
    asio::serial_port_base::stop_bits::type stopBitsType;
    switch(stopBits)
    {
        case STOPBITS_ONE:
            stopBitsType = asio::serial_port_base::stop_bits::one;
            break;
        case STOPBITS_ONE_POINT_FIVE:
            stopBitsType = asio::serial_port_base::stop_bits::onepointfive;
            break;
        case STOPBITS_TWO:
            stopBitsType = asio::serial_port_base::stop_bits::two;
            break;
        default:
            assert(0); // invalid stop bits!
            break;
    }
    port->set_option(asio::serial_port_base::stop_bits(stopBitsType));
}

void SimpleSerial::set_parity(Parity parity)
{
    asio::serial_port_base::parity::type parityType;
    switch(parity)
    {
        case PARITY_EVEN:
            parityType = asio::serial_port_base::parity::even;
            break;
        case PARITY_ODD:
            parityType = asio::serial_port_base::parity::odd;
            break;
        case PARITY_NONE:
            parityType = asio::serial_port_base::parity::none;
            break;
        default:
            assert(0); // invalid parity type!
            break;
    }
    port->set_option(asio::serial_port_base::parity(parityType));
}

void SimpleSerial::close()
{
    port->close();
}

void SimpleSerial::flush_receive_buffer()
{
    flush(TCIFLUSH);
}

void SimpleSerial::flush_send_buffer()
{
    flush(TCOFLUSH);
}

void SimpleSerial::flush_both_buffers()
{
    flush(TCIOFLUSH);
}

void SimpleSerial::flush(int flushBuffer)
{
    system::error_code error;
    if(0 == ::tcflush(port->lowest_layer().native_handle(), flushBuffer))
    {
        error = system::error_code();
    }
    else
    {
        error = system::error_code(errno,
                asio::error::get_system_category());
    }
}

int SimpleSerial::write(std::string data)
{
    return asio::write(*port, asio::buffer(data, data.size()));
}

int SimpleSerial::write_line(std::string data)
{
    return write(data + "\r\n");
}

std::string SimpleSerial::read(int numBytes)
{
    char data[numBytes];
    asio::read(*port, asio::buffer(&data,numBytes));
    return data;
}

std::string SimpleSerial::read_line() {
    std::string str;
    char c;
    while(c != '\n')
    {
        asio::read(*port, asio::buffer(&c,1));
        str += c;
    }

    return str;
}

