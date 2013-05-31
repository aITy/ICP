/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "connectdialog.h"

#include <QHostAddress>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

/**
  * connect dialog for connecting to remote game
  */

ConnectDialog::ConnectDialog(QWidget *parent)
    :QDialog(parent)
{
    setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
    /** regexp validator for port validation */
    QRegExpValidator* portValidator = new QRegExpValidator(QRegExp("^\[0-9]{5}$"), this);
    LineEdit_Port->setValidator(portValidator);
	setModal(false);
}

/**
 * function that validate and store content of dialog, when the send button is clicked
 */

bool ConnectDialog::send()
{

    QString ipaddress = LineEdit_Addr->text();
    QHostAddress address;
    if (!address.setAddress(ipaddress))
    {
        QMessageBox::critical(this,
                              trUtf8("Invalid IP address"),
                              trUtf8("Format IP adresy neni validni!"));
        LineEdit_Addr->setFocus();
        return false;
    }

    QString port_str = LineEdit_Port->text();
    /*if (LineEdit_Port->validator()->validate(port_str, port) != QValidator::Acceptable)
    {
        QMessageBox::critical(this,
                              trUtf8("Invalid port"),
                              trUtf8("Port musi obsahovat 5­ mistne cislo"));
        LineEdit_Port->setFocus();
        return false;
    }*/
    dialogInfo << ipaddress << port_str;
    //qDebug() << dialogInfo;
    emit(dialogAccepted(dialogInfo));
    return true;
}

/**
  * if send button is clicked, validation is performed and in case of need, the dialog is accepted
  */
void ConnectDialog::on_button_Send_clicked()
{
    if (send()) {
        this->accept();
    }
}

