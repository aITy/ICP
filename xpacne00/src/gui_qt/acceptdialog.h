/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef ACCEPTDIALOG_H
#define ACCEPTDIALOG_H

#include <QDialog>
#include "ui_acceptdialog.h"

class Game;

class AcceptDialog : public QDialog, Ui::acceptDialog
{
    Q_OBJECT
public:
    AcceptDialog(Game *, QWidget * parent = 0);
private:
	Game * g;
signals:
	void userAccept(Game *);
	void userReject(Game *);
private slots:
	void accepting();
	void rejecting();
};

#endif // ACCEPTDIALOG_H
