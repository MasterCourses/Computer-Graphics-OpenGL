#include "glwidget.h"
#include "window.h"
#include "ui_window.h"

window::window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::window)
{
    ui->setupUi(this);
    connect(ui->widget, SIGNAL(setXTranslate(int)), ui->xTranslateSlider, SLOT(setValue(int)));
    connect(ui->widget, SIGNAL(setJointForRedRotation(int)), ui->jointRedSlider, SLOT(setValue(int)));
    connect(ui->widget, SIGNAL(setJointForGreenRotation(int)), ui->jointGreenSlider, SLOT(setValue(int)));
    connect(ui->widget, SIGNAL(setJointForYellowRotation(int)), ui->jointYellowSlider, SLOT(setValue(int)));
}

window::~window()
{
    delete ui;
}
