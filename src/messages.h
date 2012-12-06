/*
 * messages.h
 *
 *  Created on: 06.12.2012
 *      Author: deathbringer
 */

#ifndef MESSAGES_H_152406122012
#define MESSAGES_H_152406122012

#pragma pack(1)

struct ArduinoMessage {
    uint8_t type;
    ArduinoMessage( char type ) : type(type) {}
};

struct PwmOut : public ArduinoMessage {
    enum {messageType = 0x1};
    uint8_t pin;
    uint8_t value;
    uint8_t zero;
    PwmOut(): ArduinoMessage(messageType), pin(0), value(0), zero(0){};
    PwmOut( uint8_t pin, uint8_t value )
    : ArduinoMessage(messageType), pin(pin), value(value), zero(0) {}
};

struct MoveDriveTo : public ArduinoMessage {
    enum {messageType = 0x2};
    uint8_t drive;
    uint8_t speed;
    int32_t x;
    uint8_t zero;
    MoveDriveTo()
    : ArduinoMessage(messageType), drive(0), speed(0), x(0), zero(0) {}
    MoveDriveTo(uint8_t drive, uint8_t speed, int32_t x)
    : ArduinoMessage(messageType), drive(drive), speed(speed), x(x), zero(0) {}
};

struct StopDrive : public ArduinoMessage {
    enum {messageType = 0x3};
    uint8_t drive;
    uint8_t zero;
    StopDrive()
    : ArduinoMessage(messageType), drive(0), zero(0) {}
    StopDrive(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive), zero(0) {}
};

struct ArduinoResponse{
    uint8_t result;
    ArduinoResponse(uint8_t result = 0) : result(result) {}
};

#pragma pack()


#endif /* MESSAGES_H_152406122012 */
