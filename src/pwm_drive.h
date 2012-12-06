/*
 * pwm_drive.h
 *
 *  Created on: 06.12.2012
 *      Author: deathbringer
 */

#ifndef PWM_DRIVE_H_
#define PWM_DRIVE_H_

#include "messages.h"
#include "connection.h"

class PwmDrive {
public:
    typedef uint8_t speed_type;
    typedef int32_t position_type;
    PwmDrive(ArduinoConnection & conn, uint8_t number) : speed_(0), conn(conn), number(number) {}
    void speed(speed_type v) {
        speed_ = v;
    }
    speed_type speed() const {
        return speed_;
    }
    void moveTo(position_type x) {
        ArduinoResponse response;
        if(conn.invoke(MoveDriveTo(number, speed(), x), response).result) {
            throw std::runtime_error("Drive error!");
        }
    }
    void stop() {
        ArduinoResponse response;
        if(conn.invoke(StopDrive(number), response).result) {
            throw std::runtime_error("Drive error!");
        }
    }
private:
    speed_type speed_;
    ArduinoConnection & conn;
    uint8_t number;
};

#endif /* PWM_DRIVE_H_ */
