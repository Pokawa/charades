//
// Created by hubert on 29.12.2020.
//

#include <error.h>
#include <netdb.h>
#include <Message.hpp>
#include <IncomingMessageQueue.hpp>
#include <string>
#include <QApplication>
#include "ConnectDialog.h"
#include "CommunicationHandler.hpp"


int main(int argc, char** argv){
    if (argc != 3)
        error(1, 0, "Need 2 args");

    qRegisterMetaType<chs::Message>("chs::Message");

    QApplication a(argc, argv);
    ConnectDialog dialog;
    dialog.show();

    //CharadesWindow window;
    //QObject::connect(&dialog, &ConnectDialog::accepted, [&window](){
    // auto ptr = dialog.getCommunicationHandler();
    // window.setCommunicationHandler(std::move(ptr));
    // });


    return a.exec();
}

