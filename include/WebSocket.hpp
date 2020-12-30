//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_WEBSOCKET_H
#define CHARADES_WEBSOCKET_H

#include <resolv.h>
#include <string>
#include "WebSocket.hpp"

namespace chs {

    class WebSocket {
    private:
        int descriptor;
        sockaddr_in sockInfo;

    public:
        WebSocket(int fd, sockaddr_in sockInfo);
        [[nodiscard]] std::string getAddress() const;
        [[nodiscard]] uint16_t getPort() const;
        [[nodiscard]] int getDescriptor() const;
        void close() const;

        ~WebSocket();
    };

}


#endif //CHARADES_WEBSOCKET_H
