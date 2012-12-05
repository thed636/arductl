//============================================================================
// Name        : hello.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <boost/asio.hpp>
#include <ctime>
#include <vector>
#include <iterator>
#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

using namespace std;

class ArduinoConnection {
    boost::asio::io_service io_service;
    boost::asio::serial_port port;
    std::size_t retries_;
protected:
    void waitForProgramRestart(){
        //boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        uint8_t ch(0);
        read(&ch,1);
    }
public:
    ArduinoConnection( const std::string & portName, const std::size_t baudrate = 115200 )
    : port(io_service, portName), retries_(0) {
        port.set_option(boost::asio::serial_port_base::baud_rate(115200));
        port.set_option(boost::asio::serial_port_base::character_size(8));
        port.set_option(
                boost::asio::serial_port_base::flow_control(
                        boost::asio::serial_port_base::flow_control::none));
        port.set_option(
                boost::asio::serial_port_base::parity(
                        boost::asio::serial_port_base::parity::none));
        port.set_option(
                boost::asio::serial_port_base::stop_bits(
                        boost::asio::serial_port_base::stop_bits::one));
        waitForProgramRestart();
    }

    std::size_t write( const uint8_t * out, std::size_t size ) {
        return port.write_some(boost::asio::buffer(out, size));
    }

    std::size_t read( uint8_t * in, std::size_t size ) {
        for ( std::size_t nbytes = 0;nbytes != size;) {
            if (nbytes) {
                retries_++;
            }
            nbytes += port.read_some(boost::asio::buffer(in + nbytes, size - nbytes));
        }
        return size;
    }

    std::size_t retries() const {
        return retries_;
    }
    void close() {
        port.close();
    }
};// class ArduinoConnection

//|int                |int int int
//|byte|byte|byte|byte|
//|cmd |
#pragma pack(1)
const std::size_t packetSize = 16;

struct ArduinoMessage {
    uint8_t type;
    ArduinoMessage( char type ) : type(type) {}
};

struct PwmOut : public ArduinoMessage {
    enum {messageType = 0x1, freeSize = packetSize - sizeof(ArduinoMessage) - 3};
    uint8_t pin;
    uint8_t value;
    uint8_t zero;
    uint8_t dummy[freeSize];
    PwmOut( uint8_t pin, uint8_t value ) : ArduinoMessage(messageType), pin(pin), value(value), zero(0) {}
};

struct MoveDriveTo : public ArduinoMessage {
    enum {messageType = 0x2, freeSize = packetSize - sizeof(ArduinoMessage) - 7};
    uint8_t drive;
    uint8_t speed;
    int32_t x;
    uint8_t zero;
    uint8_t dummy[freeSize];
    MoveDriveTo(uint8_t drive, uint8_t speed, int32_t x)
    : ArduinoMessage(messageType), drive(drive), speed(speed), x(x), zero(0) {}
};

struct ArduinoResponse {
    uint8_t result;
    ArduinoResponse(uint8_t result = 0) : result(result) {}
};

#pragma pack()

int main() {
    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM5");

    unsigned char in[packetSize];
    std::vector<clock_t> readTimes;
    std::vector<clock_t> writeTimes;
    const int maxi(10000);
    readTimes.reserve(maxi);
    writeTimes.reserve(maxi);
    std::time_t t(std::time(0));
    int errcnt(0);
    for (int i = 0; i != maxi; ++i) {
        const unsigned char data = i & 0xFF;
        PwmOut msg(2, data);
        std::size_t nbytes = conn.write((const uint8_t*)(&msg), packetSize);
        if (nbytes != packetSize) {
            std::cout << "nbytes=" << nbytes << std::endl;
        }
        std::fill_n(in, packetSize, 0);

        conn.read(in, packetSize);
        const ArduinoResponse & response(*((ArduinoResponse*)in));
        if (response.result) {
            std::cout << std::hex;
            std::ostream_iterator<unsigned int> outIt(cout, ", ");
            std::copy(in, in + packetSize, outIt);
            std::cout << std::dec << std::endl;
            errcnt++;
        }
    }
    //conn.close();
    const double timePerCycle = double(std::time(0) - t) / maxi;
    std::cout << "Complete!!! time = " << timePerCycle << ", speed="
            << double(packetSize) / timePerCycle * 2. / 115200 * 9 << " errors=" << errcnt
            << ", retries=" << conn.retries() << std::endl;
//    std::ostream_iterator<std::size_t> outIt(cout, ", ");

//	std::copy(writeTimes.begin(), writeTimes.end(), out);
//	std::cout << std::endl;
//	std::copy(readTimes.begin(), readTimes.end(), out);
    return 0;
}
