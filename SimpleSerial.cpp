#include <boost/asio/serial_port.hpp> 
#include <boost/asio.hpp> 
#include <memory>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace boost;

enum Parity { 
    PARITY_EVEN,
    PARITY_ODD,
    PARITY_NONE
};

class SerialPort {
    public:

        asio::io_service io;
        asio::serial_port* port;

        void open(std::string portName, long baud = 9600, int byteSize = 8, 
                Parity parity = PARITY_NONE)
        {
            port = new asio::serial_port(io);
            port->open(portName);
            port->set_option(asio::serial_port_base::baud_rate(baud));

            port->set_option(boost::asio::serial_port_base::character_size(byteSize));
            //port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
            setParity(parity);
            //port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            //port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        }

        void setParity(Parity parity)
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
            }
            port->set_option(boost::asio::serial_port_base::parity(parityType));
        }

        void close()
        {
            port->close();
        }

        void flush_receive_buffer()
        {
            flush(TCIFLUSH);
        }

        void flush_send_buffer()
        {
            flush(TCOFLUSH);
        }

        void flush_both_buffers()
        {
            flush(TCIOFLUSH);
        }

        void flush(int flushBuffer)
        {
            boost::system::error_code error;
            if(0 == ::tcflush(port->lowest_layer().native_handle(), flushBuffer))
            {
                error = boost::system::error_code();
            }
            else
            {
                error = boost::system::error_code(errno,
                        boost::asio::error::get_system_category());
            }
        }

        int write(std::string data)
        {
            return asio::write(*port, asio::buffer(data, data.size()));
        }

        void write_line(std::string data)
        {
            write(data + "\r\n");
        }

        std::string read(int numBytes)
        {
            //std::string data;
            char data[numBytes];
            asio::read(*port, asio::buffer(&data,numBytes));
            return data;
        }

        std::string read_line() {
            std::string str;
            char c;
            while(c != '\n')
            {
                asio::read(*port, asio::buffer(&c,1));
                str += c;
            }

            return str;
        }
};

int main()
{
    SerialPort p;
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

