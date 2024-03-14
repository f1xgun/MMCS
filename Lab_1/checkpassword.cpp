#include "checkpassword.h"
#include "qclipboard.h"
#include "ui_checkpassword.h"

CheckPassword::CheckPassword(QWidget *parent, std::function<QString(const QString&)> on_accept_callback, std::function<void(void)> on_failed_callback)
    : QWidget(parent)
    , ui(new Ui::CheckPassword), on_accept_callback(on_accept_callback), on_failed_callback(on_failed_callback)
{
    ui->setupUi(this);
}

CheckPassword::~CheckPassword()
{
    delete ui;
}

void CheckPassword::on_accept_button_clicked()
{
    QString password = ui->password_input->text();
    QString decrypted_creds = this->on_accept_callback(password);
    if (decrypted_creds == "ERROR") {
        this->on_failed_callback();
        return;
    }
    QGuiApplication::clipboard()->setText(decrypted_creds);
}

