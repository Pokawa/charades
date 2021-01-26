#include "CharadesWindow.hpp"
#include "ui_CharadesWindow.h"
#include <fmt/format.h>
#include <QColorDialog>
#include <utility>

CharadesWindow::CharadesWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::CharadesWindow),
        gameState(GameState::WAITING_FOR_INFO)
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
    connect(ui->drawWidget, &DrawWidget::linePainted, communicationHandler.get(), &CommunicationHandler::drawLineRequest);
    connect(communicationHandler.get(), &CommunicationHandler::linePainted, ui->drawWidget, &DrawWidget::drawLine);
    connect(communicationHandler.get(), &CommunicationHandler::drawingCleared, ui->drawWidget, &DrawWidget::clear);
    connect(communicationHandler.get(), &CommunicationHandler::receivedChatMessage, this, &CharadesWindow::printChatMessage);
    connect(communicationHandler.get(), &CommunicationHandler::receivedServerMessage, this, &CharadesWindow::printServerMessage);

    connect(communicationHandler.get(), &CommunicationHandler::joiningRoomFailed,
            [this](){this->ui->roomsRespondLabel->setText("Joining failed, room doesn't exist"); });

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
    communicationHandler->sendChatMessageRequest(ui->chatInput->text().toStdString());
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
    ui->stackedWidget->setCurrentIndex(1);
}

void CharadesWindow::handleInGameInfoRespond(chs::Message message) {
    auto [owner, joinedNames, joinedScores, gameIsActive, startPoint, drawer, wordCount] =
            chs::deconstructMessage<std::string, std::string, std::string, bool, std::chrono::time_point<std::chrono::system_clock>, std::string, int>(message);

    spdlog::info("owner: {} drawer: {}", owner, drawer);

    if (gameIsActive and gameState != GameState::PLAYING) {
        roundStartingPoint = startPoint;

        if (drawer == username) {
            ui->colorButton->setDisabled(false);
            ui->clearButton->setDisabled(false);
            ui->drawWidget->setDisabled(false);
        } else {
            ui->charadesLabel->setText(QString::fromStdString(fmt::format("Number of words: {} currently drawing: {}", wordCount, drawer)));
            ui->chatInput->setDisabled(false);
        }

        ui->startButton->setDisabled(true);
        gameState = GameState::PLAYING;
    }

    if (not gameIsActive) {
        ui->chatInput->setDisabled(true);
        ui->charadesLabel->clear();
        gameState = GameState::WAITING_FOR_START;

        if (owner == username) {
            ui->startButton->setDisabled(false);
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
