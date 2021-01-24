#include "CharadesWindow.hpp"
#include "ui_CharadesWindow.h"
#include <fmt/format.h>
#include <QColorDialog>

CharadesWindow::CharadesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CharadesWindow)
{
    ui->setupUi(this);
}

CharadesWindow::~CharadesWindow()
{
    delete ui;
}

void CharadesWindow::on_newRoomButton_clicked()
{
    communicationHandler->newRoomRequest();
}

void CharadesWindow::on_refreshButton_clicked()
{
    ui->roomsList->clear();
    communicationHandler->roomsInfoRequest();
}

CharadesWindow::CharadesWindow(std::unique_ptr<CommunicationHandler> ptr) : CharadesWindow() {
    communicationHandler = std::move(ptr);
    connect(communicationHandler.get(), &CommunicationHandler::roomsInfoRespond, this, &CharadesWindow::roomsInfoRespond);
    //connect joinRoomRespond;
    connect(communicationHandler.get(), &CommunicationHandler::joiningRoomFailed,
            [this](){this->ui->roomsRespondLabel->setText("Joining failed, room doesn't exist"); });

    emit ui->refreshButton->clicked();
}

void CharadesWindow::roomsInfoRespond(int roomNumber, std::string joinedNames) {
    std::replace(joinedNames.begin(), joinedNames.end(), ';', ' ');
    auto itemString = fmt::format("Room {} Players: {}", roomNumber, joinedNames);
    ui->roomsList->addItem(QString::fromStdString(itemString));
}

void CharadesWindow::on_roomsList_itemDoubleClicked(QListWidgetItem *item)
{
    auto roomNumber = item->text().split(' ')[1].toInt();
    spdlog::info("Requested to join room {}", roomNumber);
    communicationHandler->joinRoomRequest(roomNumber);
}

void CharadesWindow::on_colorButton_clicked()
{
    auto color = QColorDialog::getColor(ui->drawWidget->drawColor(), nullptr, "Pick a color");
    ui->drawWidget->setDrawColor(color);
}

void CharadesWindow::on_chatInput_editingFinished()
{
    spdlog::info("editing finished!");
}

void CharadesWindow::on_exitButton_clicked()
{

}

void CharadesWindow::on_startButton_clicked()
{

}

void CharadesWindow::on_clearButton_clicked()
{
    ui->drawWidget->clear();
    //todo send clear message
}
