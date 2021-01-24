//
// Created by hubert on 29.12.2020.
//

#include <QApplication>
#include "App.hpp"


int main(int argc, char** argv){
    qRegisterMetaType<chs::Message>("chs::Message");
    QApplication a(argc, argv);
    App app;
    app.loginAndStart();
    return a.exec();
}

