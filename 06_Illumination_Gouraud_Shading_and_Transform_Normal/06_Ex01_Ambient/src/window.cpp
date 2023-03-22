#include "window.h"
#include "ui_window.h"


window::window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::window)
{
    ui->setupUi(this);
    QWidget::setMouseTracking(true);
    ui->widget->setMouseTracking(true);
}

window::~window()
{
    delete ui;
}

