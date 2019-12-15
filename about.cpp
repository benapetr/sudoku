// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#include "about.h"
#include "global.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About)
{
    this->ui->setupUi(this);
    this->ui->label_2->setText(APP_VERSION);
}

About::~About()
{
    delete this->ui;
}
