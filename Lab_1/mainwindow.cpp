#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "homewindow.h"
#include <QCryptographicHash>

std::string RIGHT_PASSWORD = "123";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_button_clicked()
{
    QByteArray key = QCryptographicHash::hash(
        ui->password_input->text().toUtf8(),
        QCryptographicHash::Sha256).toHex();
    RecordsManager* records_manager = new RecordsManager();
    if (records_manager->load_records(key)) {
        ui->error_text->setText("");
        this->hide();
        HomeWindow* home_window = new HomeWindow(this, records_manager);
        home_window->show();
    }
    else {
        delete records_manager;
        ui->error_text->setText("Неверный пароль!");
    }
}

