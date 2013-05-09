/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef HELP_H
#define HELP_H

#include <QDialog>
#include "ui_help.h"

class Help : public QDialog,Ui::helpDialog {
    Q_OBJECT
public:
    Help(QWidget *parent = 0);
    ~Help();
};

#endif // HELP_H
