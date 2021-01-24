//
// Created by hubert on 24.01.2021.
//

#ifndef CHARADES_APP_HPP
#define CHARADES_APP_HPP


#include <memory>
#include "ConnectDialog.h"
#include "CharadesWindow.hpp"

class App : public QObject {
    Q_OBJECT

    std::unique_ptr<ConnectDialog> connectDialog;
    std::unique_ptr<CharadesWindow> charadesWindow;


public slots:
    void loginAndStart();
    void openCharadesWindow();
};


#endif //CHARADES_APP_HPP
