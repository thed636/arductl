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
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        uint8_t ch[4] = {0, 0, 0, 0};
        port.read_some(boost::asio::buffer(ch, 4));
        //receive(ch);
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

    std::size_t send( const uint8_t * out, std::size_t size ) {
        return port.write_some(boost::asio::buffer(out, size));
    }

    template <typename Request>
    std::size_t send( const Request & request ) {
        return send(reinterpret_cast<const uint8_t*>(&request), sizeof(request));
    }

    std::size_t receive( uint8_t * in, std::size_t size ) {
        for ( std::size_t nbytes = 0;nbytes != size;) {
            if (nbytes) {
                retries_++;
            }
            nbytes += port.read_some(boost::asio::buffer(in + nbytes, size - nbytes));
        }
        return size;
    }

    template<typename Response >
    Response & receive(Response & response) {
        receive(reinterpret_cast<uint8_t*>(&response), sizeof(response));
        return response;
    }

    template< typename Request, typename Response >
    Response & invoke( const Request & request, Response & response) {
        send(request);
        return receive(response);
    }

    std::size_t retries() const {
        return retries_;
    }

    void close() {
        port.close();
    }
};// class ArduinoConnection

#endif /* CONNECTION_H_ */
