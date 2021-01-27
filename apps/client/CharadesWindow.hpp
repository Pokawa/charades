#ifndef CHARADESWINDOW_HPP
#define CHARADESWINDOW_HPP

#include <QMainWindow>
#include <QListWidgetItem>
#include "CommunicationHandler.hpp"

namespace Ui {
class CharadesWindow;
}

class CharadesWindow : public QMainWindow
{
    Q_OBJECT

    std::unique_ptr<CommunicationHandler> communicationHandler;
    std::chrono::time_point<std::chrono::system_clock> roundStartingPoint;
    std::string username;
    int numberOfPlayers;
    enum class GameState {
        PLAYING,
        WAITING_FOR_INFO,
        WAITING_FOR_START
    } gameState;

public:
    explicit CharadesWindow(QWidget *parent = nullptr);
    explicit CharadesWindow(std::unique_ptr<CommunicationHandler> ptr, std::string username);
    ~CharadesWindow() override;

private slots:
    void on_newRoomButton_clicked();
    void roomsInfoRespond(chs::Message message);
    void on_refreshButton_clicked();
    void on_roomsList_itemDoubleClicked(QListWidgetItem *item);
    void on_colorButton_clicked();
    void on_chatInput_editingFinished();
    void on_exitButton_clicked();
    void on_startButton_clicked();
    void on_clearButton_clicked();
    void on_drawingCheckbox_stateChanged(int arg1);
    void joinedRoom();
    void handleInGameInfoRespond(chs::Message message);
    void disableRoomControls();
    void printChatMessage(const std::string& message);
    void printServerMessage(const std::string& message);
    void printCharadesWordForDrawer(const std::string& message);

private:
    Ui::CharadesWindow *ui;

};

#endif // CHARADESWINDOW_HPP
