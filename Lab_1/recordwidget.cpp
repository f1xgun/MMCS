#include "recordwidget.h"
#include "ui_recordwidget.h"

RecordWidget::RecordWidget(const std::string& site, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
    ui->site_name->setText(QString::fromStdString(site));
}

RecordWidget::~RecordWidget()
{
    delete ui;
}

QSize RecordWidget::sizeHint() const {
    return QSize(600, 100);
}
