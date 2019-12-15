// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

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

void Options::SetAutoload(bool value)
{
    Options::settings->setValue("autoload", value);
}

QString Options::LastFile()
{
    return Options::settings->value("last-file", "").toString();
}

void Options::SetLastFile(QString path)
{
    Options::settings->setValue("last-file", path);
}

void Options::SetRemoveHints(bool set)
{
    Options::settings->setValue("remove-hints", set);
}

bool Options::GetRemoveHints()
{
    return Options::settings->value("remove-hints", true).toBool();
}

bool Options::GetAssistedMode()
{
    return Options::settings->value("assisted-mode", false).toBool();
}

void Options::SetAssistedMode(bool set)
{
    Options::settings->setValue("assisted-mode", set);
}

QString Options::GetLastDir()
{
    return Options::settings->value("last-dir").toString();
}

void Options::SetLastDir(QString name)
{
    Options::settings->setValue("last-dir", name);
}
