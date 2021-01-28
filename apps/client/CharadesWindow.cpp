#include "CharadesWindow.hpp"
#include "ui_CharadesWindow.h"
#include <fmt/format.h>
#include <QColorDialog>
#include <utility>

CharadesWindow::CharadesWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CharadesWindow),
        numberOfPlayers(0)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
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

CharadesWindow::CharadesWindow(std::unique_ptr<CommunicationHandler> ptr, std::string username) : CharadesWindow() {
    this->username = std::move(username);
    communicationHandler = std::move(ptr);
    connect(communicationHandler.get(), &CommunicationHandler::roomsInfoRespond, this, &CharadesWindow::roomsInfoRespond);
    connect(communicationHandler.get(), &CommunicationHandler::joinedRoom, this, &CharadesWindow::joinedRoom);
    connect(communicationHandler.get(), &CommunicationHandler::inGameInfoRespond, this, &CharadesWindow::handleInGameInfoRespond);
    connect(communicationHandler.get(), &CommunicationHandler::linePainted, ui->drawWidget, &DrawWidget::drawLine);
    connect(communicationHandler.get(), &CommunicationHandler::drawingCleared, ui->drawWidget, &DrawWidget::clear);
    connect(communicationHandler.get(), &CommunicationHandler::receivedChatMessage, this, &CharadesWindow::printChatMessage);
    connect(communicationHandler.get(), &CommunicationHandler::receivedServerMessage, this, &CharadesWindow::printServerMessage);
    connect(communicationHandler.get(), &CommunicationHandler::receivedCharadesWord, this, &CharadesWindow::printCharadesWordForDrawer);
    connect(communicationHandler.get(), &CommunicationHandler::joiningRoomFailed,
            [this](){this->ui->roomsRespondLabel->setText("Joining failed, room doesn't exist"); });

    connect(ui->drawWidget, &DrawWidget::linePainted, communicationHandler.get(), &CommunicationHandler::drawLineRequest);

    emit ui->refreshButton->clicked();

    disableRoomControls();
}

void CharadesWindow::roomsInfoRespond(chs::Message message) {
    auto[roomNumber, joinedNames] = chs::deconstructMessage<int, std::string>(message);
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
    auto input = ui->chatInput->text();
    if (!input.isEmpty()) {
        communicationHandler->sendChatMessageRequest(input.toStdString());
        ui->chatInput->clear();
    }
}

void CharadesWindow::on_exitButton_clicked()
{
    communicationHandler->exitRoomRequest();
    ui->stackedWidget->setCurrentIndex(0);
    emit ui->refreshButton->clicked();
}

void CharadesWindow::on_startButton_clicked()
{
    communicationHandler->startGameRequest();
}

void CharadesWindow::on_clearButton_clicked()
{
    ui->drawWidget->clear();
    communicationHandler->clearRequest();
}

void CharadesWindow::on_drawingCheckbox_stateChanged(int state)
{
    if (state == Qt::Checked) {
        communicationHandler->enterDrawingQueueRequest();
    }

    if (state == Qt::Unchecked) {
        communicationHandler->leaveDrawingQueueRequest();
    }
}

void CharadesWindow::joinedRoom() {
    ui->drawWidget->clear();
    ui->charadesLabel->clear();
    ui->chatInput->clear();
    ui->chatWindow->clear();
    ui->playersScoresList->clear();
    ui->drawingCheckbox->setCheckState(Qt::Unchecked);
    ui->stackedWidget->setCurrentIndex(1);
}

void CharadesWindow::handleInGameInfoRespond(chs::Message message) {
    auto [owner, joinedNames, joinedScores, gameIsActive, startPoint, drawer, wordCount] =
            chs::deconstructMessage<std::string, std::string, std::string, bool, std::chrono::time_point<std::chrono::system_clock>, std::string, int>(message);

    if (gameIsActive) {
        roundStartingPoint = startPoint;
        disableRoomControls();

        if (drawer == username) {
            ui->colorButton->setEnabled(true);
            ui->clearButton->setEnabled(true);
            ui->drawWidget->setEnabled(true);
        } else {
            ui->charadesLabel->setText(QString::fromStdString(fmt::format("Number of words: {} currently drawing: {}", wordCount, drawer)));
            ui->chatInput->setEnabled(true);
        }

    }

    auto names = chs::explodeJoinedString(joinedNames, ';');
    auto scores = chs::explodeJoinedString(joinedScores, ';');

    ui->playersScoresList->clear();
    for (int i = 0; i < names.size(); ++i) {
        auto* item = new QListWidgetItem(QString::fromStdString(fmt::format("{} : {} points", names[i], scores[i])));
        if (names[i] == owner) {
            auto font = item->font();
            font.setBold(true);
            item->setFont(font);
        }
        ui->playersScoresList->addItem(item);
    }

    numberOfPlayers = names.size();

    if (not gameIsActive) {
        disableRoomControls();
        ui->charadesLabel->clear();

        if (owner == username and numberOfPlayers >= 2) {
            ui->startButton->setEnabled(true);
        }
    }


}

void CharadesWindow::disableRoomControls() {
    ui->colorButton->setDisabled(true);
    ui->clearButton->setDisabled(true);
    ui->drawWidget->setDisabled(true);
    ui->chatInput->setDisabled(true);
    ui->startButton->setDisabled(true);
}

void CharadesWindow::printChatMessage(const std::string &message) {
    ui->chatWindow->appendPlainText(QString::fromStdString(message));
}

void CharadesWindow::printServerMessage(const std::string &message) {
    auto serverMessage = fmt::format("<b>{}</b>", message);
    ui->chatWindow->appendHtml(QString::fromStdString(serverMessage));
}

void CharadesWindow::printCharadesWordForDrawer(const std::string & word) {
    auto labelText = fmt::format("Word: {}", word);
    ui->charadesLabel->setText(QString::fromStdString(labelText));
}
