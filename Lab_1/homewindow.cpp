#include "homewindow.h"
#include "ui_homewindow.h"

#include <QStandardItemModel>
#include <QListWidget>
#include <QLabel>
#include <QClipboard>
#include <QCryptographicHash>
#include "createrecorddialog.h"
#include "accountrecord.h"
#include "recordwidget.h"

HomeWindow::HomeWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HomeWindow), records_manager(new RecordsManager())
{
    ui->setupUi(this);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::load_records() {
    std::vector<AccountRecord*> records = this->records_manager->get_records();
    set_records(records);
}

void HomeWindow::set_records(std::vector<AccountRecord*> records) {
    if (!records.empty()) {
        ui->records_widget->clear();
        for (size_t i = 0; i != records.size(); ++i) {
            QListWidgetItem* record_widget = new QListWidgetItem();
            RecordWidget* item_widget = new RecordWidget(records[i]->get_site_name());
            record_widget->setSizeHint(item_widget->sizeHint());
            ui->records_widget->addItem(record_widget);
            ui->records_widget->setItemWidget(record_widget, item_widget);
        }
    }
    else {
        ui->records_widget->clear();
    }
}

void HomeWindow::on_search_button_clicked() {
    this->records_manager->filter_records(ui->search_input->text().toStdString());
    std::vector<AccountRecord*> filtered_records = this->records_manager->get_records();
    set_records(filtered_records);
}


void HomeWindow::on_add_record_button_clicked() {
    auto create_record_dialog = new CreateRecordDialog(this, [this](AccountRecord& record) { on_create_record(record);});
    create_record_dialog->show();
}

void HomeWindow::on_create_record(AccountRecord& record) {
    std::string encrypted_login = this->records_manager->encrypt_string(record.get_login());
    record.set_login(encrypted_login);
    std::string encrypted_password = this->records_manager->encrypt_string(record.get_password());
    record.set_password(encrypted_password);
    this->records_manager->add_record(record);
    this->records_manager->filter_records(ui->search_input->text().toStdString());
    std::vector<AccountRecord*> records = this->records_manager->get_records();
    set_records(records);
}


void HomeWindow::on_records_widget_doubleClicked(const QModelIndex &index)
{
    this->cur_record_index = index.row();
    ui->stackedWidget->setCurrentIndex(0);
}


void HomeWindow::on_login_button_clicked()
{
    QByteArray key = QCryptographicHash::hash(
                         ui->password_input->text().toUtf8(),
                         QCryptographicHash::Sha256).toHex();

    if (is_first_login) {
        if (this->records_manager->load_records(key)) {
            ui->error_text->setText("");
            ui->stackedWidget->setCurrentIndex(1);
            load_records();
            is_first_login = false;
        }
        else {
            ui->error_text->setText("Неверный пароль!");
        }
    } else {
        QString decrypted_creds = this->records_manager->get_record_cred(key, this->cur_record_index);
        if (decrypted_creds == "ERROR") {
            ui->error_text->setText("Неверный пароль!");
            return;
        }
        ui->error_text->setText("");
        QGuiApplication::clipboard()->setText(decrypted_creds);
        ui->stackedWidget->setCurrentIndex(1);
    }
    ui->password_input->clear();
    ui->records_widget->setCurrentRow(-1);
    ui->copy_record_button->setEnabled(false);
    ui->delete_record_button->setEnabled(false);
}

void HomeWindow::on_records_widget_itemClicked(QListWidgetItem *item)
{
    this->cur_record_index = item->listWidget()->currentRow();
    ui->copy_record_button->setEnabled(true);
    ui->delete_record_button->setEnabled(true);
}


void HomeWindow::on_copy_record_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void HomeWindow::on_search_input_textChanged(const QString &arg1)
{
    on_search_button_clicked();
}

void HomeWindow::closeEvent(QCloseEvent* event) {
    if (!is_first_login) {
       this->records_manager->save_records();
    }
}


void HomeWindow::on_delete_record_button_clicked()
{
    this->records_manager->delete_record(this->cur_record_index);
    load_records();
    this->cur_record_index = -1;
    ui->records_widget->setCurrentRow(-1);
    ui->copy_record_button->setEnabled(false);
    ui->delete_record_button->setEnabled(false);
}

