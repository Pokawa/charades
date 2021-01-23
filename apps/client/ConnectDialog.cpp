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

    auto host = ui->addressInput->text().toStdString();
    auto port = ui->portInput->value();

    if (communicationHandler != nullptr) {
        communicationHandler->disconnectFromHost();
    }

    auto ret = CommunicationHandler::connectToHost(host, port);
    if (ret == -1) {
        ui->respondLabel->setText(strerror(errno));
        return;
    }

    communicationHandler = std::make_shared<CommunicationHandler>(ret);
    connect(communicationHandler.get(), &CommunicationHandler::loginSuccessful, this, &ConnectDialog::accept);
    connect(communicationHandler.get(), &CommunicationHandler::loginFailed, [this](){ this->ui->respondLabel->setText("Login failed, username in use"); });

    auto username = ui->usernameInput->text().toStdString();

    if (username.find(';') != std::string::npos) {
        ui->respondLabel->setText("Invalid username, semicolon is forbidden");
        return;
    }

    communicationHandler->logInRequest(username);
}

void ConnectDialog::on_ConnectDialog_accepted() {
    auto username = ui->usernameInput->text().toStdString();
    spdlog::info("login accepted as {}", username);
    //TODO open main window
}
