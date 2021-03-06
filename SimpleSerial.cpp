#include "SimpleSerial.h"

using namespace boost;

void SimpleSerial::open(std::string portName, long baud, ByteSize byteSize, 
        Parity parity, StopBits stopBits,
        FlowControl flowControl)
{
    port = std::unique_ptr<asio::serial_port>(new asio::serial_port(io));
    port->open(portName);

    // Set all serial port properties
    port->set_option(asio::serial_port_base::baud_rate(baud));
    set_byte_size(byteSize);
    set_parity(parity);
    set_stop_bits(stopBits);
    set_flow_control(flowControl);
}

void SimpleSerial::close()
{
    port->close();
}

bool SimpleSerial::is_open()
{
    return port->is_open();
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
        c = char(read(1)[0]);
        str += c;
    }

    return str;
}

void SimpleSerial::set_byte_size(ByteSize byteSize)
{
    int bitsPerByte;
    switch(byteSize)
    {
        case FIVE_BITS:
            bitsPerByte = 5;
            break;
        case SIX_BITS:
            bitsPerByte = 6;
            break;
        case SEVEN_BITS:
            bitsPerByte = 7;
            break;
        case EIGHT_BITS:
            bitsPerByte = 8;
            break;
        default:
            assert(0); // invalid byte size!
            break;
    }
    port->set_option(asio::serial_port_base::character_size(bitsPerByte));
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
        throw std::runtime_error("Failed to flush serial port: " + error.message());
    }
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

