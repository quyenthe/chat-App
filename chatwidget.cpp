#include "chatwidget.h"
#include "ui_chatwidget.h"

chatwidget::chatwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatwidget)
{
    ui->setupUi(this);
}

chatwidget::~chatwidget()
{
    delete ui;
}
