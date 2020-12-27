//
// Created by hubert on 27.12.2020.
//

//#ifndef CHARADES_WEBSOCKET_H
//#define CHARADES_WEBSOCKET_H
#pragma once

#include <resolv.h>
#include <string>
#include "WebSocket.h"

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
    };

}




//#endif //CHARADES_WEBSOCKET_H