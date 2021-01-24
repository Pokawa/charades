#include "CharadesWindow.hpp"
#include "ui_CharadesWindow.h"

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

void CharadesWindow::on_joinButton_clicked()
{

}

void CharadesWindow::on_newRoomButton_clicked()
{

}
