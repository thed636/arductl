//============================================================================
// Name        : hello.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctime>
#include <vector>
#include <iterator>
#include <algorithm>
#include "pwm_drive.h"
#include "connection.h"

void move(ArduinoConnection & conn) {
    PwmDrive drive(conn,1);
    drive.speed(70);
    drive.moveTo(1);
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    drive.speed(200);
    drive.moveTo(-1);
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    drive.stop();
}

int main() {
    std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM7");
    move(conn);
/*
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
 */
    return 0;
}
