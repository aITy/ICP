/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include "ui_connectdialog.h"

class ConnectDialog : public QDialog, private Ui::connectDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    QStringList dialogInfo;
private:
    bool send();

private slots:
    void on_button_Send_clicked();
signals:
    void dialogAccepted(QStringList);
};


#endif // CONNECTDIALOG_H
