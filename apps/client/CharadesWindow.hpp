#ifndef CHARADESWINDOW_HPP
#define CHARADESWINDOW_HPP

#include <QMainWindow>
#include "CommunicationHandler.hpp"

namespace Ui {
class CharadesWindow;
}

class CharadesWindow : public QMainWindow
{
    Q_OBJECT

    std::unique_ptr<CommunicationHandler> communicationHandler;

public:
    explicit CharadesWindow(QWidget *parent = nullptr);
    explicit CharadesWindow(std::unique_ptr<CommunicationHandler> ptr);
    ~CharadesWindow();

private slots:
    void on_joinButton_clicked();
    void on_newRoomButton_clicked();
    void roomsInfoRespond(int roomNumber, std::string joinedNames);

    void on_refreshButton_clicked();

private:
    Ui::CharadesWindow *ui;
};

#endif // CHARADESWINDOW_HPP
