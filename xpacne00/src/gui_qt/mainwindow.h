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
#include "canvas.h"

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


/**
 * MainWindow object - singleton
 */

class MainWindow : public QMainWindow, Ui::MainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    static MainWindow * getInstance(){ return instance; }

    /** add game function and function that return games count*/
    void addGame(GameBoard * );
	void removeLastGame();
	void removeGame(int);
    int getGamesCount();

    /** sets status bar msg functions */
    void setStatusMsg(const char *);
    void setStatusMsg(QString);

    /** clear scene */
    void clearMoves();
    /** set replay buttons */
    void toggleReplayButtons(Game *);
    /** set moves notification area content */
    void setLineEditText(const QString &);
    typedef enum {
      COLOR_DONT_KNOW,
      COLOR_WHITE,
      COLOR_BLACK,
    } color_t;

protected:
    void closeEvent(QCloseEvent *);

private:
    /** static instance if this object */
    static MainWindow* instance;

    /** setup actions from gui menus */
    void setupActions();

    /** moves on the all board */
    QList< QPair<GameBoard*, QPair< QPair<QPair<QString, int>, QPair<QString, int> >, QString> > > moves;
    /** moves notification area content of all board */
    QList< QPair< GameBoard*, QString> > moves_str;
    /** current board */
    QList<GameBoard*> games_arr;
    /** server */
    QTcpServer * server;
private slots:
    /** got connection slot */
	void incomingConnection();
    void gotConnection(Game *);
    /** user disconnected slot */
    void gotExitSlot();
	void gotRejected();
    void newNetworkGame(QStringList list);
    void gotInviteSlot(Player::color_t color, QString str);
    /** replay buttons slots */
    void on_spinBox_Delay_valueChanged(int );
    void on_pushButton_Forward_clicked();
    void on_pushButton_Pause_clicked();
    void on_pushButton_Rewind_clicked();
    void on_pushButton_Play_clicked();
    void on_pushButton_Stop_clicked();
    /** advice button */
     void on_pushButton_Advice_clicked();
    /** current tab has changed */
    void on_tabWidget_Games_currentChanged(int index);
    /** reimplemented function to provide confirming when user try to close the tab */
    void on_tabWidget_Games_tabCloseRequested(int index);
    /** menu actions */
    void createLocalCpu();
    void createLocalVs();
    void showNewNetDialog();
    void openFromFile();
    void openReplayFromFile();
    void help();
    bool saveIcp();
    bool saveXml();
	void inviteAccepted(Game *);
	void inviteRejected(Game *);
};
#endif // MAINWINDOW_H
