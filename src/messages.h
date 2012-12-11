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
    int16_t x;
    uint8_t zero;
    MoveDriveTo()
    : ArduinoMessage(messageType), drive(0), speed(0), x(0), zero(0) {}
    MoveDriveTo(uint8_t drive, uint8_t speed, int16_t x)
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

struct GetDrivePosition : public ArduinoMessage {
    enum {messageType = 0x4};
    uint8_t drive;
    uint8_t zero;
    GetDrivePosition()
    : ArduinoMessage(messageType), drive(0), zero(0) {}
    GetDrivePosition(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive), zero(0) {}
};

struct GetDrivePositionError : public ArduinoMessage {
    enum {messageType = 0x5};
    uint8_t drive;
    uint8_t zero;
    GetDrivePositionError()
    : ArduinoMessage(messageType), drive(0), zero(0) {}
    GetDrivePositionError(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive), zero(0) {}
};

struct GetDriveSpeed : public ArduinoMessage {
    enum {messageType = 0x6};
    uint8_t drive;
    uint8_t zero;
    GetDriveSpeed()
    : ArduinoMessage(messageType), drive(0), zero(0) {}
    GetDriveSpeed(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive), zero(0) {}
};

#define TRANSFER_FLOAT( name )\
    int32_t name##_;\
    float name() const { return float(name##_) / float(scaleFactor);}\
    void name( float v ) { name##_ = v * float(scaleFactor);}

struct ConfigureDrive : public ArduinoMessage {
    enum {messageType = 0x7};
    enum { scaleFactor = 10000 };
    uint8_t drive;
    struct PositionRegulator {
        TRANSFER_FLOAT( Kp );
    } pos;

    struct SpeedRegulator {
        TRANSFER_FLOAT( Kp )
        TRANSFER_FLOAT( Kip );
        TRANSFER_FLOAT( Kdp )
    } speed;
    struct Pwm {
        int16_t deadZone;
    } pwm;
    ConfigureDrive() : ArduinoMessage(messageType), drive(0) {}
    ConfigureDrive( uint8_t number, float Kpp, float Kp, float Kip, float Kdp, int16_t deadZone )
    : ArduinoMessage(messageType), drive(number) {
        speed.Kp(Kp);
        speed.Kip(Kip);
        speed.Kdp(Kdp);
        pos.Kp(Kpp);
        pwm.deadZone = deadZone;
    }
};

struct GetDriveState : public ArduinoMessage {
    enum {messageType = 0x8};
    uint8_t drive;
    GetDriveState()
    : ArduinoMessage(messageType), drive(0) {}
    GetDriveState(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive) {}
};

struct MoveDriveSpeed : public ArduinoMessage {
    enum {messageType = 0x9};
    uint8_t drive;
    int16_t speed;
    MoveDriveSpeed()
    : ArduinoMessage(messageType), drive(0), speed(0) {}
    MoveDriveSpeed(uint8_t drive, int16_t speed)
    : ArduinoMessage(messageType), drive(drive), speed(speed) {}
};

struct SeekDriveSpeed : public ArduinoMessage {
    enum {messageType = 0xA};
    uint8_t drive;
    int16_t speed;
    SeekDriveSpeed()
    : ArduinoMessage(messageType), drive(0), speed(0) {}
    SeekDriveSpeed(uint8_t drive, int16_t speed)
    : ArduinoMessage(messageType), drive(drive), speed(speed) {}
};

struct ResetDrive : public ArduinoMessage {
    enum {messageType = 0xB};
    uint8_t drive;
    ResetDrive()
    : ArduinoMessage(messageType), drive(0) {}
    ResetDrive(uint8_t drive)
    : ArduinoMessage(messageType), drive(drive) {}
};

struct ArduinoResponse{
    uint8_t result;
    ArduinoResponse(uint8_t result = 0) : result(result) {}
};

struct DrivePosition : ArduinoResponse{
    int16_t pos;
    DrivePosition(int16_t pos=0, uint8_t result = 0) : ArduinoResponse(result), pos(pos) {}
};


struct DrivePositionError : ArduinoResponse{
    int16_t error;
    DrivePositionError(int16_t error=0, uint8_t result = 0) : ArduinoResponse(result), error(error) {}
};

struct DriveSpeed : ArduinoResponse{
    int16_t speed;
    DriveSpeed(int16_t speed=0, uint8_t result = 0) : ArduinoResponse(result), speed(speed) {}
};

struct DriveState {
    int16_t pos;
    int16_t error;
    int16_t speed;
    int16_t speedError;
    int16_t out;
    DriveState() : pos(0), error(0), speed(0), speedError(0), out(0) {}
};

struct DriveStateResponse : ArduinoResponse{
    DriveState state;
    DriveStateResponse( const DriveState & state, uint8_t result = 0 )
    : ArduinoResponse(result), state(state) {}
    DriveStateResponse() : ArduinoResponse(0) {}
};

#pragma pack()


#endif /* MESSAGES_H_152406122012 */
