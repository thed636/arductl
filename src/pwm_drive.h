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
#include "invoker.h"
class PwmDrive {
public:
    typedef int16_t speed_type;
    typedef int16_t position_type;

    PwmDrive(ArduinoConnection & conn, uint8_t number) : conn(conn), number(number) {}

    void moveTo(position_type x) {
        Invoker<MoveTo>(conn,number,x);
    }

    void moveSpeed(speed_type speed) {
        Invoker<MoveSpeed>(conn,number,speed);
    }

    void seek(speed_type speed) {
        Invoker<SeekDriveSpeed>(conn,number,speed);
    }

    void reset() {
        Invoker<ResetDrive>(conn,number);
    }

    void stop() {
        moveSpeed(0);
    }

    void configure( float Kpp, float Kp, float Kip, float Kdp, int16_t deadZone = 50 ) {
        Invoker<ConfigureDrive>(conn, number, Kpp, Kp, Kip, Kdp, deadZone );
    }

    DriveState state() const {
        return Invoker<GetDriveState>(conn,number).response().state;
    }

    speed_type speed() const {
        return state().speed;
    }

    position_type position() const {
        return state().pos;
    }

    position_type positionError() const {
        return state().error;
    }

private:
    ArduinoConnection & conn;
    uint8_t number;
};

#endif /* PWM_DRIVE_H_ */
