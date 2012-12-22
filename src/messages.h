/*
 * messages.h
 *
 *  Created on: 06.12.2012
 *      Author: deathbringer
 */

#ifndef MESSAGES_H_152406122012
#define MESSAGES_H_152406122012

#pragma pack(1)

struct ArduinoRequest {
    uint8_t type;
    ArduinoRequest( char type ) : type(type) {}
};

struct ArduinoResponse{
    typedef uint8_t result_type;
    result_type result;
    ArduinoResponse(result_type result = 0) : result(result) {}
};

struct PwmOut {
    struct Request : public ArduinoRequest {
        enum {messageType = 0x1};
        uint8_t pin;
        uint8_t value;
        Request(): ArduinoRequest(messageType), pin(0), value(0){};
        Request( uint8_t pin, uint8_t value )
        : ArduinoRequest(messageType), pin(pin), value(value){}
    };
    typedef ArduinoResponse Response;
};

struct DriveRequest : public ArduinoRequest {
    uint8_t drive;
    DriveRequest( uint8_t type, uint8_t drive = 0 )
    : ArduinoRequest(type), drive(drive) {}
};

struct DriveMessage {
    typedef DriveRequest Request;
    typedef ArduinoResponse Response;
};

struct MoveTo : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum {messageType = 0x2};
        int16_t x;
        Request() : Base::Request(messageType), x(0) {}
        Request(uint8_t drive, int16_t x)
        : Base::Request(messageType, drive), x(x) {}
    };
};

struct MoveSpeed : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum {messageType = 0x5};
        int16_t speed;
        Request()
        : Base::Request(messageType), speed(0) {}
        Request(uint8_t drive, int16_t speed)
        : Base::Request(messageType, drive), speed(speed) {}
    };
};

struct SeekDriveSpeed : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum {messageType = 0x6};
        int16_t speed;
        Request()
        : Base::Request(messageType), speed(0) {}
        Request(uint8_t drive, int16_t speed)
        : Base::Request(messageType, drive), speed(speed) {}
    };
};

struct ResetDrive : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum {messageType = 0x7};
        Request() : Base::Request(messageType){}
        Request(uint8_t drive) : Base::Request(messageType, drive) {}
    };
};


struct DriveState {
    int16_t pos;
    int16_t error;
    int16_t speed;
    int16_t speedError;
    int16_t out;
    DriveState() : pos(0), error(0), speed(0), speedError(0), out(0) {}
};


struct GetDriveState : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum {messageType = 0x4};
        Request() : Base::Request(messageType){}
        Request(uint8_t drive) : Base::Request(messageType, drive) {}
    };
    struct Response : Base::Response{
        DriveState state;
        Response( const DriveState & state, uint8_t result = 0 )
        : Base::Response(result), state(state) {}
        Response(){}
    };
};



#define TRANSFER_FLOAT( name )\
    int32_t name##_;\
    float name() const { return float(name##_) / float(scaleFactor);}\
    void name( float v ) { name##_ = v * float(scaleFactor);}

struct ConfigureDrive : public DriveMessage {
    typedef DriveMessage Base;
    struct Request : public Base::Request {
        enum { messageType = 0x3 };
        enum { scaleFactor = 10000 };

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

        Request() : Base::Request(messageType) {}
        Request( uint8_t number, float Kpp, float Kp, float Kip, float Kdp, int16_t deadZone )
        : Base::Request(messageType, number) {
            speed.Kp(Kp);
            speed.Kip(Kip);
            speed.Kdp(Kdp);
            pos.Kp(Kpp);
            pwm.deadZone = deadZone;
        }
    };
};














#pragma pack()


#endif /* MESSAGES_H_152406122012 */
