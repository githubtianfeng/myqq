#include "Music.h"
#include "ui_Music.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QMenu>

Music::Music(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Music)
{
    ui->setupUi(this);
    this->setMaximumSize(880,621);
    this->setMinimumSize(880,621);
}

Music::~Music()
{
    delete ui;
}

void Music::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Music::on_toolButton_8_clicked()
{

}

void Music::on_toolButton_7_clicked()
{

}

void Music::on_toolButton_6_clicked()
{
    close();
}

void Music::on_pushButton_clicked()
{
   QStringList songsList=QFileDialog::getOpenFileNames(this,tr("选择文件"),QDir::homePath(),tr("*.mp3"));
   for(int i=0;i<songsList.size();i++)
   {
       QString song=songsList.at(i);
     //  this->playList->addMedia(QUrl::fromLocalFile(song));
       QStringList songL=song.split('/');
       song=songL.last();
       ui->songList->addItem(song);
   }
}
