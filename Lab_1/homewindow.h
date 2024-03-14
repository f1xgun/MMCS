#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

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
    explicit HomeWindow(QWidget *parent = nullptr, RecordsManager* records_manager = new RecordsManager());
    ~HomeWindow();

private slots:
    void on_search_button_clicked();

    void on_add_record_button_clicked();

    void on_records_widget_doubleClicked(const QModelIndex &index);

private:
    Ui::HomeWindow *ui;
    RecordsManager* records_manager;

    void load_records();
    void set_records(std::vector<AccountRecord> records);
    void on_create_record(AccountRecord& record);
};

#endif // HOMEWINDOW_H
