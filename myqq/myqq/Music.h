#ifndef MUSIC_H
#define MUSIC_H

#include <QMainWindow>

namespace Ui {
    class Music;
}

class Music : public QMainWindow {
    Q_OBJECT
public:
    Music(QWidget *parent = 0);
    ~Music();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Music *ui;

private slots:
    void on_pushButton_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_7_clicked();
    void on_toolButton_8_clicked();
};

#endif // MUSIC_H
