#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About)
{
    this->ui->setupUi(this);
}

About::~About()
{
    delete this->ui;
}