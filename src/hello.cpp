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
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        s = drive.state();
    }
    std::cout << s <<  std::endl;
}

bool driveMoveComplete(const DriveState & s) {
    return s.error==0 && s.speedError==0;
}

bool driveMoveInomplete(const DriveState & s) {
    return !driveMoveComplete(s) ;
}

DriveState getState( const PwmDrive & drive ) {
    return drive.state();
}

template <typename Iter>
void waitForExecDrives(Iter first, Iter last) {
    std::vector<DriveState> states;
    std::transform(first, last, std::back_inserter(states), &getState);
    while( std::find_if(states.begin(), states.end(), &driveMoveInomplete ) != states.end())  {
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        std::ostream_iterator<DriveState> out(std::cout, "\n");
        std::copy(states.begin(), states.end(), out);
        states.clear();
        std::transform(first, last, std::back_inserter(states), &getState);
    }
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


void moveTo(PwmDrive & drive, int16_t target) {
    drive.moveTo(target);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

void moveSpeed(PwmDrive & drive, int16_t speed) {
    drive.moveSpeed(speed);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

void calibrate(PwmDrive & drive) {
    drive.moveTo(50);
    waitForExec(drive);
    drive.seek(-60);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    drive.reset();
}

void measure(PwmDrive & drive){
    drive.moveTo(40);
    waitForExec(drive);
    drive.seek(40);
    waitForExec(drive);
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
}

int main() {
    std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
    ArduinoConnection conn("COM7");
    std::vector<PwmDrive> drives = { PwmDrive(conn,1), PwmDrive(conn,2), PwmDrive(conn,3), PwmDrive(conn,4), PwmDrive(conn,5) };
    drives[0].configure( 4.5, 2, 2, 0.1, 65 );
    drives[1].configure( 5, 2, 2, 0.1, 95 );
    drives[2].configure( 6, 2, 2, 0.1, 75 );
    drives[3].configure( 3.5, 2, 1.5, 0.1, 65 );
    drives[4].configure( 1, 0.15, 40, 0.001, 120 );
    printHeader( std::cout ) << std::endl;
    //move(conn);
    //moveSpeed(drive2, 10);
    //drives[4].moveTo(-7);
    //moveTo(drives[3], 5);
    //moveTo(drives[2], -240);
//    moveTo(drives[0], -50);

    calibrate(drives[0]);
    drives[0].moveTo(-180);
    drives[1].moveTo(-120);
    drives[2].moveTo(240);
    drives[3].moveTo(-95);
    waitForExecDrives(drives.begin(), drives.end());
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    for (auto drive : drives) {
        drive.moveTo(0);
    }
    waitForExecDrives(drives.begin(), drives.end());


//    moveTo(drives[3], 5);
//    moveTo(drives[0], 0);
    //printHeader( std::cout ) << std::endl;
    //measure(drive);
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
