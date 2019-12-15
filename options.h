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
        static bool GetAssistedMode();
        static void SetAssistedMode(bool set);
        static QString GetLastDir();
        static void SetLastDir(QString name);

    private:
        static QSettings *settings;
};

#endif // OPTIONS_H
