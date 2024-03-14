#ifndef CREATERECORDDIALOG_H
#define CREATERECORDDIALOG_H

#include <QDialog>
#include "accountrecord.h"

namespace Ui {
class CreateRecordDialog;
}

class CreateRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRecordDialog(QWidget *parent = nullptr, std::function<void(AccountRecord&)> on_create_button_callback = nullptr);
    ~CreateRecordDialog();


private slots:
    void on_create_push_button_clicked();

    void on_cancel_push_button_clicked();

private:

    Ui::CreateRecordDialog *ui;
    std::function<void(AccountRecord&)> on_create_button_callback;
};

#endif // CREATERECORDDIALOG_H
