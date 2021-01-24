#ifndef CHARADESWINDOW_HPP
#define CHARADESWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class CharadesWindow;
}

class CharadesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CharadesWindow(QWidget *parent = nullptr);
    ~CharadesWindow();

private slots:
    void on_joinButton_clicked();

    void on_newRoomButton_clicked();

private:
    Ui::CharadesWindow *ui;
};

#endif // CHARADESWINDOW_HPP
