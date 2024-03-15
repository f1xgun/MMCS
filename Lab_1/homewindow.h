#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "qlistwidget.h"
#include "recordsmanager.h"
#include <QDialog>
#include "accountrecord.h"

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

private slots:
    void on_search_button_clicked();

    void on_add_record_button_clicked();

    void on_records_widget_doubleClicked(const QModelIndex &index);

    void on_login_button_clicked();

    void on_records_widget_itemClicked(QListWidgetItem *item);

    void on_copy_record_button_clicked();

    void on_search_input_textChanged(const QString &arg1);

    void closeEvent(QCloseEvent* event);
    void on_delete_record_button_clicked();

private:
    Ui::HomeWindow *ui;
    RecordsManager* records_manager;
    bool is_first_login = true;
    int cur_record_index = -1;

    void load_records();
    void set_records(std::vector<AccountRecord*> records);
    void on_create_record(AccountRecord& record);
};

#endif // HOMEWINDOW_H
