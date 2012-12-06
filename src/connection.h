/*
 * connection.h
 *
 *  Created on: 06.12.2012
 *      Author: deathbringer
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_


#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

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

    template< typename Request, typename Response >
    Response & invoke( const Request & request, Response & response) {
        write((const uint8_t*)(&request), sizeof(request));
        read((uint8_t*)(&response), sizeof(response));
        return response;
    }
};// class ArduinoConnection

#endif /* CONNECTION_H_ */
