// Created by Petr Bena <petr@bena.rocks> (c) 2019, all rights reserved

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QHash>

class QSettings;

class Options
{
    public:
        static void Initialize();
        static bool Autoload();
        static void SetAutoload(bool value);
        static QString LastFile();
        static void SetLastFile(QString path);
        static void SetRemoveHints(bool set);
        static bool GetRemoveHints();

    private:
        static QSettings *settings;
};

#endif // OPTIONS_H
