#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"
#include <spdlog/spdlog.h>

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
    auto host = ui->addressInput->text().toStdString();
    auto port = std::to_string(ui->portInput->value());
    bool result = true;

    if (result) {
        spdlog::info("Connected");
        emit accept();
    }
}
