#include "homewindow.h"
#include "checkpassword.h"
#include "mainwindow.h"
#include "ui_homewindow.h"

#include <QStandardItemModel>
#include <QListWidget>
#include <QLabel>
#include <QClipboard>
#include "createrecorddialog.h"
#include "accountrecord.h"
#include "recordwidget.h"

HomeWindow::HomeWindow(QWidget *parent, RecordsManager* records_manager)
    : QDialog(parent)
    , ui(new Ui::HomeWindow), records_manager(records_manager)
{
    ui->setupUi(this);
    load_records();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::load_records() {
    std::vector<AccountRecord> records = this->records_manager->get_records();
    set_records(records);
}

void HomeWindow::set_records(std::vector<AccountRecord> records) {
    if (!records.empty()) {
        ui->records_widget->clear();
        for (size_t i = 0; i != records.size(); ++i) {
            QListWidgetItem* record_widget = new QListWidgetItem();
            RecordWidget* item_widget = new RecordWidget(records[i].get_site_name());
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
    std::vector<AccountRecord> filtered_records = this->records_manager->filter_records(ui->search_input->text().toStdString());
    set_records(filtered_records);
}


void HomeWindow::on_add_record_button_clicked() {
    auto create_record_dialog = new CreateRecordDialog(this, [this](AccountRecord& record) { on_create_record(record);});
    create_record_dialog->show();
}

void HomeWindow::on_create_record(AccountRecord& record) {
    this->records_manager->add_record(record);
    std::vector<AccountRecord> records = this->records_manager->filter_records(ui->search_input->text().toStdString());
    set_records(records);
}


void HomeWindow::on_records_widget_doubleClicked(const QModelIndex &index)
{
    auto check_password_dialog = new CheckPassword(this, [this, index](const QString& password) {
        return this->records_manager->get_record_cred(password, index.row());
        }, [this]{
            this->hide();
        }
    );
    check_password_dialog->show();
}

