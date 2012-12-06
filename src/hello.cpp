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

void waitForExec(PwmDrive & drive) {
    for(int error = drive.error(); error; error = drive.error())  {
        boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        std::cout << "error=" << error << ", speed=" << drive.actualSpeed() <<  std::endl;
    }
}

void move(ArduinoConnection & conn) {
    PwmDrive drive(conn,1);
    const int target = 100;
    drive.speed(70);
    drive.moveTo(target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    drive.moveTo(-target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

    drive.speed(200);
    drive.moveTo(0);
    waitForExec(drive);
    std::cout << drive.position() << std::endl;
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

int main() {
    std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM7");
    move(conn);

    return 0;
}
