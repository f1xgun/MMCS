#include "createrecorddialog.h"
#include "ui_createrecorddialog.h"
#include <QDialog>

CreateRecordDialog::CreateRecordDialog(QWidget *parent, std::function<void(AccountRecord&)> on_create_button_callback)
    : QDialog(parent), ui(new Ui::CreateRecordDialog)
    , on_create_button_callback(on_create_button_callback)
{
    ui->setupUi(this);
}

CreateRecordDialog::~CreateRecordDialog()
{
    delete ui;
}

void CreateRecordDialog::on_create_push_button_clicked()
{
    std::string site_name = ui->site_name_input->text().toStdString();
    std::string login = ui->login_input->text().toStdString();
    std::string password = ui->password_input->text().toStdString();

    if (!site_name.empty() && !login.empty() && !password.empty()) {
        AccountRecord new_record = AccountRecord(site_name, login, password);
        this->on_create_button_callback(new_record);
    }
    else {
        if (site_name.empty()) {
            qDebug() << "Create record error: site_name is empty!";
        }
        if (login.empty()) {
            qDebug() << "Create record error: login is empty!";
        }
        if (password.empty()) {
            qDebug() << "Create record error: password is empty!";
        }
    }

    this->hide();
}


void CreateRecordDialog::on_cancel_push_button_clicked()
{
    this->hide();
}

