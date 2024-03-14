#ifndef CHECKPASSWORD_H
#define CHECKPASSWORD_H

#include <QWidget>

namespace Ui {
class CheckPassword;
}

class CheckPassword : public QWidget
{
    Q_OBJECT

public:
    explicit CheckPassword(QWidget *parent = nullptr, std::function<QString(const QString&)> on_accept_callback = nullptr, std::function<void(void)> on_failed_callback = nullptr);
    ~CheckPassword();

private slots:
    void on_accept_button_clicked();

private:
    Ui::CheckPassword *ui;

    std::function<QString(const QString&)> on_accept_callback;

    std::function<void(void)> on_failed_callback;
};

#endif // CHECKPASSWORD_H
