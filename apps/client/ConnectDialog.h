#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include "CommunicationHandler.hpp"

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ConnectDialog *ui;
    std::unique_ptr<CommunicationHandler> communicationHandler;

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog() override;
    std::unique_ptr<CommunicationHandler> getCommunicationHandler();
    std::string getUserName();

public slots:
    void setConnectionLostLabel();

private slots:
    void on_connectButton_clicked();
    void on_ConnectDialog_accepted();

};

#endif // CONNECTDIALOG_H
