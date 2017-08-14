#ifndef CLASSMOULD_H
#define CLASSMOULD_H
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QFont>
#include <QMouseEvent>

//#define CLASS       "/usr/local/shencloud/image1/ClassName1.png"
//#define CLASSPRESS   "/usr/local/shencloud/image1/ClassName1Press.png"

#define CLASS      "/usr/local/shencloud/image1/1.png"
#define CLASSPRESS      "/usr/local/shencloud/image1/4.png"
#define CLASSPIC        "/usr/local/shencloud/image1/3.png"
#define CLASSENTER      "/usr/local/shencloud/image1/2.png"

#define WINDOWSXP    "/usr/local/shencloud/image1/windowsxp.png"
#define WINDOWS7     "/usr/local/shencloud/image1/windows7.png"
#define LINUX        "/usr/local/shencloud/image1/linux.png"

class ClassMould:public QLabel
{
    Q_OBJECT
public:
    explicit ClassMould(QWidget *parent,QString name);
    void SetChecked(bool flag);
    void SetClassName(QString name);
    bool IsChecked();
    void SetCheckedFlag(bool flag);
private:
    QLabel  *m_pClassPic;
    QLabel  *m_pShowName;
    QString  m_strClassName;
    bool  m_bIsChecked;
    QFont   m_Font;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
signals:
    void LabelChecked();
    void LabelDoubleclicked();
};

#endif // CLASSMOULD_H
