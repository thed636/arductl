/*
 * invoker.h
 *
 *  Created on: 13.12.2012
 *      Author: deathbringer
 */

#ifndef INVOKER_H_
#define INVOKER_H_

#include <utility>

template <typename T>
class Invoker {
public:
    typedef typename T::Request Request;
    typedef typename T::Response Response;
    typedef typename Response::result_type result_type;
    typedef ArduinoConnection Conn;
    const Response & response() {
        return response_;
    }

    template<typename ...Args>
    Invoker(Conn & conn, Args&&... params) {
        handleResult(conn.invoke(Request(std::forward<Args>(params)...), response_).result);
    }
protected:
    void handleResult(const result_type & result) {
        if(result) {
            throw std::runtime_error("Drive error!");
        }
    }
private:
    Response response_;
};


#endif /* INVOKER_H_ */
