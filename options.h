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
        static QString LastFile();
        static void SetLastFile(QString path);

    private:
        static QSettings *settings;
};

#endif // OPTIONS_H
