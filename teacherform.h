#ifndef TEACHERFORM_H
#define TEACHERFORM_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QComboBox>
#include <QPushButton>
#include "mylable.h"
#include <QTabWidget>
#include <QScrollArea>
#include "classmould.h"
#include "teachervm.h"
#include <QFont>
#include <QBoxLayout>

#define LOGOPNG1 "/home/pengjian/QtProject/CloudClassRoom/image/Logo2.png"
#define TEACHERNAMEPNG "/home/pengjian/QtProject/CloudClassRoom/image/TeacherName.png"
#define CLASSNAMEPNG    "/home/pengjian/QtProject/CloudClassRoom/image/ClassName.png"
#define ClASSSTATUSPNG  "/home/pengjian/QtProject/CloudClassRoom/image/ClassStatus.png"

#define STUDYSELFPNG    "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StudySelp.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StudySelpPress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StudySelp.png)}"

#define CLASSOVER       "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ClassOver.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ClassOverPress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ClassOver.png)}"

#define STARTCLASS      "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartClass.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartClassPress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/StartClass.png)}"

#define EXITPNG         "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Exit.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/ExitPress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Exit.png)}"


#define RIGHTPNG        "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Right.png);}"\
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/RightPress.png);}"\
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Right.png)}"

#define LEFTPNG         "QPushButton{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Left.png);}" \
                        "QPushButton:hover{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/LeftPress.png);}" \
                        "QPushButton:pressed{border-image: url(/home/pengjian/QtProject/CloudClassRoom/image/Left.png)}"
#define CLASSNAME1      "/home/pengjian/QtProject/CloudClassRoom/image/ClassName1.png"
#define CLASSNAME2      "/home/pengjian/QtProject/CloudClassRoom/image/ClassName2.png"


namespace Ui {
class TeacherForm;
}

class TeacherForm : public QWidget
{
    Q_OBJECT

public:
    QLCDNumber *m_pTimeLCDNumber;
public:
    explicit TeacherForm(QWidget *parent = 0);
    ~TeacherForm();
    void SetTeachetName(QString Name);
    void SetClassRoom(QString number);
private slots:
    void on_BackpushButton_clicked();

private:
    Ui::TeacherForm *ui;
    QLabel  *m_pTeacherName;
    QLabel  *m_pClassRoomNum;
    QLabel  *m_pLogoLabel;
    QLabel  *m_pTeacherNameLogoTabel;
    QPushButton     *m_pBackPushButton;
    QLabel  *m_pTeachetVMStatusTabel;

    QLabel  *m_pClassNameLabel;
    QLabel  *m_pClassNameLogoLabel;
    QLabel  *m_pClassNamelTitletabel;

    QPushButton *m_pStartSelpPushButton;
    QPushButton *m_pClassOverPushButton;
    QLabel      *m_pGrandName;
    QString m_strTeacherName;
    QString m_strClassRoomNum;
    myLable *m_pMyLable[10];
    QWidget     *m_pStudentLabelContent;
    QComboBox   *m_pComboxGrand;
    QTabWidget   *m_pClassTabWidget;
    QPushButton     *m_pRightPushButton;
    QPushButton     *m_pLeftPushButton;
    QLabel          *m_pClassNameLabel_1;
    QLabel          *m_pClassNameLabel_2;
    QLabel          *m_pClassNameLabel_3;
    QPushButton     *m_pClassStartPushButton;
    QScrollArea     *m_pTeacherVMScrollArea;
    QWidget     *m_pTeachetVMQWidget;
    TeacherVM   *m_pTeacherVM[10];
    QFont       m_Font;
//    QVBoxLayout *m_pTeacherVMLayOut;
//    QGridLayout *m_pTeacherVMGridLayOut;
};

#endif // TEACHERFORM_H
