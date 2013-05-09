/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef ACCEPTDIALOG_H
#define ACCEPTDIALOG_H

#include <QDialog>
#include "ui_acceptdialog.h"

class AcceptDialog : public QDialog, Ui::acceptDialog
{
public:
    AcceptDialog(QWidget * parent = 0);
};

#endif // ACCEPTDIALOG_H
