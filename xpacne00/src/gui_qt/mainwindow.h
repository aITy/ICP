/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "../share/game.h"
#include "gameboard.h"
#include "connectdialog.h"
#include "acceptdialog.h"
#include "help.h"

#include <QApplication>
#include <QList>
#include <QTcpServer>
#include <QCloseEvent>
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStatusBar>
#include <QDebug>


class MainWindow : public QMainWindow, Ui::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    MainWindow * getMainWindow() {
        return instance;
    }
    static MainWindow * getInstance();
    void addGame(GameBoard * );
    int getGamesCount();
    void setStatusMsg(const char *);
    void setStatusMsg(QString);
    void clearMoves();
    void toggleReplayButtons(Game *);
    void setLineEditText(QString str);
    typedef enum {
      COLOR_DONT_KNOW,
      COLOR_WHITE,
      COLOR_BLACK,
    } color_t;

protected:
    void closeEvent(QCloseEvent *);

private:
    static MainWindow* instance;
    void setupActions();

    QList< QPair<GameBoard*, QPair< QPair<QPair<QString, int>, QPair<QString, int> >, QString> > > moves;
    QList< QPair< GameBoard*, QString> > moves_str;
    QList<GameBoard*> games_arr;
    QTcpServer * server;
    QList<int> ports;
    ConnectDialog *d;
private slots:
    void gotConnection();
    void gotInviteSlot(Player::color_t, QString);
    void gotExitSlot();
    void on_spinBox_Delay_valueChanged(int );
    void on_pushButton_Forward_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Rewind_clicked();
    void on_pushButton_Play_clicked();
    void on_pushButton_Stop_clicked();
    void on_tabWidget_Games_currentChanged(int index);
    void createLocalCpu();
    void createLocalVs();
    void showNewNetDialog();
    void openFromFile();
    void openReplayFromFile();
    void help();
    bool saveIcp();
    bool saveXml();
    void on_tabWidget_Games_tabCloseRequested(int index);
    void newNetworkGame(QStringList);
signals:
    void listen(void);
};
#endif // MAINWINDOW_H
