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

std::ostream & printHeader( std::ostream & s ) {
    return s << "pos\terror\tspeed\tspeedError\tout";
}

std::ostream & operator << (std::ostream & s, const DriveState & state) {
    return s << state.pos << '\t'<< state.error << '\t'<< state.speed << '\t'<< state.speedError << '\t'<< state.out;
}

void waitForExec(PwmDrive & drive) {
    DriveState s(drive.state());
    while( s.error || s.speedError )  {
        std::cout << s <<  std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(3));
        s = drive.state();
    }
    std::cout << s <<  std::endl;
}

void move(ArduinoConnection & conn) {
    PwmDrive drive(conn,1);
    const int target = 500;
    drive.configure( 5., 4., .003, .0 );
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
    drive.configure( 10, .5, 2, 0.01, 60 );
    drive.moveTo(target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

void moveSpeed(ArduinoConnection & conn, int16_t speed) {
    PwmDrive drive(conn,1);
    drive.configure( 0.5, .5, 1, 0.01, 60 );
    drive.moveSpeed(speed);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

void calibrate(ArduinoConnection & conn) {
    PwmDrive drive(conn,1);
    drive.configure( 5, 0.6, 4, 0.005, 60 );
    drive.moveTo(50);
    waitForExec(drive);
    drive.seek(60);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    drive.reset();
}

void measure(ArduinoConnection & conn){
    PwmDrive drive(conn,1);
    drive.moveTo(40);
    waitForExec(drive);
    drive.seek(60);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

int main() {
    std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM7");
    printHeader( std::cout ) << std::endl;
    //move(conn);
    //moveSpeed(conn, 60);
    moveTo(conn, 10);
    //calibrate(conn);
    //printHeader( std::cout ) << std::endl;
    //measure(conn);
    //moveSpeed(conn, 140);
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
