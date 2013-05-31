/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "acceptdialog.h"
#include <QAbstractButton>


AcceptDialog::AcceptDialog(Game * game, QWidget *parent)
        : QDialog(parent)
{
    g = game;
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	QAbstractButton * ok = buttonBox->buttons().at(0);
	QAbstractButton * no = buttonBox->buttons().at(1);
	connect(ok, SIGNAL(clicked()), this, SLOT(accepting()));
	connect(no, SIGNAL(clicked()), this, SLOT(rejecting()));
}

void AcceptDialog::accepting() {
	emit(userAccept(g));
	this->accept();
}

void AcceptDialog::rejecting() {
	emit(userReject(g));
	this->reject();
}
