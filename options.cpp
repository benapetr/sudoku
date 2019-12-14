#include "options.h"
#include <QSettings>

QSettings *Options::settings = nullptr;

void Options::Initialize()
{
    Options::settings = new QSettings();
}

bool Options::Autoload()
{
    return Options::settings->value("autoload", true).toBool();
}

QString Options::LastFile()
{
    return Options::settings->value("last-file", "").toString();
}

void Options::SetLastFile(QString path)
{
    Options::settings->setValue("last-file", path);
}
