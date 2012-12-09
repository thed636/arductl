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

std::ostream printHeader( std::ostream & s ) {
    return s << "pos\terror\tspeed\tspeedError\tout";
}

std::ostream & operator << (std::ostream & s, const DriveState & state) {
    return s << state.pos << '\t'<< state.error << '\t'<< state.speed << '\t'<< state.speedError << '\t'<< state.out;
}

void waitForExec(PwmDrive & drive) {
    DriveState s(drive.state());
    while( s.error && s.speedError )  {
        std::cout << s <<  std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        s = drive.state();
    }
    std::cout << s <<  std::endl;
}

void move(ArduinoConnection & conn) {
    PwmDrive drive(conn,1);
    printHeader( std::cout ) << std::endl;
    const int target = 500;
    drive.configure( 5., 5., .0005, .05 );
    //drive.speed(70);
    drive.moveTo(target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//
//    drive.moveTo(-target);
//    waitForExec(drive);
//    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//
//    drive.speed(200);
//    drive.moveTo(0);
//    waitForExec(drive);
//    std::cout << drive.position() << std::endl;
//    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}


void moveTo(ArduinoConnection & conn, int16_t target) {
    PwmDrive drive(conn,1);
    drive.speed(65);
    drive.moveTo(target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}
int main() {
    std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM7");
    move(conn);
//    int16_t target = 0;
//    while(target>-15000) {
//        std::cout << "Enter target:";
//        std::cin >> target;
//        if(target>-15000)
//            moveTo(conn, target);
//        while( true ) {
//            PwmDrive drive(conn,1);
//            std::cout << "error=" << drive.error() << ", position="<< drive.position() << ", speed=" << drive.actualSpeed() <<  std::endl;
//        }
//    }

    return 0;
}
