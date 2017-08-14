#include "loginwidget.h"
#include <QApplication>
#include "ryanConfig.h"
#include "include.h"
#include "waitstu2.h"
#include "stwidget.h"
#include <QFontDatabase>
#include <QFont>
#include <QString>


extern void *WhileFun(void *param);
QString loadFontFromFile(QString path)
{
    static QString font;
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        int loadedFontID = QFontDatabase::addApplicationFont(path);
        QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
        if(!loadedFontFamilies.empty())
            font = loadedFontFamilies.at(0);
    }
    return font;
}

int main(int argc, char *argv[])
{
    int iRecode = 0;
    while((iRecode = getopt(argc,argv,"vV")) != EOF)
    {

        switch (iRecode) {
            case 'v':
            case 'V':
            {
                printf(VISION_VISON,__DATE__,__TIME__);
                return 0;
            }
        }
    }
    QApplication a(argc, argv);
    LoginWidget w;
    w.showFullScreen();
    QString fontName = loadFontFromFile("/usr/local/share/fonts/msyh.ttc");
	QFont font(fontName);
	QApplication::setFont(font);
    //w.show();
    //stwidget st;
    //st.show();
    return a.exec();
}
