#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"
#include <spdlog/spdlog.h>
#include <QLabel>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}


void ConnectDialog::on_connectButton_clicked()
{
    ui->respondLabel->setText("");
    auto username = ui->usernameInput->text().toStdString();

    if (username.find(';') != std::string::npos) {
        ui->respondLabel->setText("Invalid username, semicolon is forbidden");
        return;
    }

    if (username.find(' ') != std::string::npos) {
        ui->respondLabel->setText("Invalid username, spaces are forbidden");
        return;
    }

    if (username.empty()) {
        ui->respondLabel->setText("Invalid username");
        return;
    }

    auto host = ui->addressInput->text().toStdString();
    auto port = ui->portInput->value();

    auto ret = CommunicationHandler::connectToHost(host, port);
    if (ret == -1) {
        ui->respondLabel->setText(strerror(errno));
        return;
    }

    communicationHandler = std::make_unique<CommunicationHandler>(ret);
    connect(communicationHandler.get(), &CommunicationHandler::loginSuccessful, this, &ConnectDialog::accept);
    connect(communicationHandler.get(), &CommunicationHandler::loginFailed, [this](){ this->ui->respondLabel->setText("Login failed, username in use"); });


    communicationHandler->logInRequest(username);
}

void ConnectDialog::on_ConnectDialog_accepted() {
    auto username = ui->usernameInput->text().toStdString();
    spdlog::info("login accepted as {}", username);
}

std::unique_ptr<CommunicationHandler> ConnectDialog::getCommunicationHandler() {
    return std::move(communicationHandler);
}

std::string ConnectDialog::getUserName() {
    return ui->usernameInput->text().toStdString();
}

void ConnectDialog::setConnectionLostLabel() {
    ui->respondLabel->setText("Connection lost ( ͡° ʖ̯ ͡°)");
}
