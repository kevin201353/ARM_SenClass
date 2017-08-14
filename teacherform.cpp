#include "teacherform.h"
#include "ui_teacherform.h"

TeacherForm::TeacherForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherForm)
{
    ui->setupUi(this);
    m_Font.setPixelSize(20);
    m_pLogoLabel = ui->Logolabel;
    m_pLogoLabel->setAlignment(Qt::AlignCenter);
    m_pLogoLabel->setPixmap(QPixmap(LOGOPNG1));

    m_pTeacherNameLogoTabel = ui->TeacherNameLogolabel;
    //m_pTeacherNameLogoTabel->setText("教师名称");
    m_pTeacherNameLogoTabel->setPixmap(QPixmap(TEACHERNAMEPNG));


    m_pBackPushButton = ui->BackpushButton;
 //   m_pBackPushButton->setText("后退");
    m_pBackPushButton->setStyleSheet(QStringLiteral(EXITPNG));
    m_pBackPushButton->setMinimumSize(26,27);

    m_pTeachetVMStatusTabel = ui->TeacherVMStatuslabel;
    m_pTeachetVMStatusTabel->setFont(m_Font);
    m_pTeachetVMStatusTabel->setAlignment(Qt::AlignCenter);
    m_pTeachetVMStatusTabel->setText("教师虚拟机:3台     可用:1台");

    m_pClassNamelTitletabel = ui->ClassNamelTitletabel;
    m_pClassNamelTitletabel->setFont(m_Font);
    m_pClassNamelTitletabel->setText("当前课程: ");

    m_pClassNameLogoLabel = ui->ClassNameLogolabel;
    m_pClassNameLogoLabel->setPixmap(QPixmap(CLASSNAMEPNG));

    m_pClassNameLabel = ui->ClassNamelabel;
    m_pClassNameLabel->setFont(m_Font);
    m_pClassNameLabel->setText("数学课");

//    m_pClassStatusLogoLabel = ui->ClassStatusLogolabel;
//    m_pClassStatusLogoLabel->setPixmap(QPixmap(ClASSSTATUSPNG));

//    m_pClassStatusLabel = ui->ClassStatuslabel;
//    m_pClassStatusLabel->setFont(ftClassName);
//    m_pClassStatusLabel->setText("自习课");



    m_pStartSelpPushButton = ui->StartSelfpushButton;
    m_pStartSelpPushButton->setFlat(false);
    //m_pStartSelpPushButton->setText("自习");
    m_pStartSelpPushButton->setStyleSheet(QStringLiteral(STUDYSELFPNG));
    m_pStartSelpPushButton->setMinimumSize(113,40);

    m_pClassOverPushButton = ui->ClassOverpushButton;
    m_pClassOverPushButton->setFlat(false);
    //m_pClassOverPushButton->setText("下课");
    m_pClassOverPushButton->setStyleSheet(QStringLiteral(CLASSOVER));
    m_pClassOverPushButton->setMinimumSize(113,40);

    m_pGrandName = ui->SetClassNamelabel;
    m_pGrandName->setFont(m_Font);
    m_pGrandName->setText("当前班级:");

    m_pTeacherName = ui->TeacherNamelabel;
    m_pClassRoomNum = ui->RoomNumberlabel;
    m_Font.setPointSize(15);
    m_pTeacherName->setFont(m_Font);
    m_pClassRoomNum->setFont(m_Font);

    m_pTeacherName->setText(m_strTeacherName);
    m_pClassRoomNum->setText(m_strClassRoomNum);

    m_pTimeLCDNumber = ui->TimelcdNumber;
    m_pTimeLCDNumber->setDigitCount(25);
    m_pTimeLCDNumber->setMode(QLCDNumber::Dec);
    m_pTimeLCDNumber->setSegmentStyle(QLCDNumber::Flat);
    m_pTimeLCDNumber->setFrameShape(QLCDNumber::NoFrame);

    m_pComboxGrand = ui->ClsaaNamecomboBox;
    m_pComboxGrand->setEditable(true);
    m_pComboxGrand->setMinimumWidth(150);
    m_pComboxGrand->setMinimumHeight(30);
    m_pClassTabWidget = ui->ClasstabWidget;
    m_Font.setPixelSize(15);
    m_pClassTabWidget->setFont(m_Font);
    m_pClassTabWidget->setTabText(0,"课程");
    m_pClassTabWidget->setTabText(1,"学生终端");
    m_pClassTabWidget->setCurrentIndex(0);

    m_pRightPushButton = ui->RightpushButton;
    m_pRightPushButton->setMinimumSize(91,169);
    m_pRightPushButton->setFlat(false);
   // m_pRightPushButton->setStyleSheet(QStringLiteral(RIGHTPNG));
    m_pRightPushButton->setStyleSheet(RIGHTPNG);
    //m_pRightPushButton->setText("右翻");
    m_pLeftPushButton = ui->LeftpushButton;
    m_pLeftPushButton->setMinimumSize(91,169);
    m_pLeftPushButton->setFlat(false);
    //m_pLeftPushButton->setStyleSheet(QStringLiteral(LEFTPNG));
    m_pLeftPushButton->setStyleSheet(LEFTPNG);
    // m_pLeftPushButton->setText("左翻");

    m_pClassStartPushButton = ui->ClassBeginpushButton;
    m_pClassStartPushButton->setMinimumSize(221,77);
    m_pClassStartPushButton->setFlat(false);
    m_pClassStartPushButton->setStyleSheet(QStringLiteral(STARTCLASS));
    //m_pClassStartPushButton->setText("上课");

    m_pTeacherVMScrollArea = ui->TeacherscrollArea;
    m_pStudentLabelContent = ui->Student_scrollAreaWidgetContents;
    m_pStudentLabelContent->setMinimumSize(1920,1080);
    m_pMyLable[0] = new myLable(m_pStudentLabelContent);
    m_pMyLable[0]->move(100,100);
    m_pMyLable[0]->RemberPos(100,100);
    m_pMyLable[0]->Enable(true);

    m_pMyLable[1] = new myLable(m_pStudentLabelContent);
    m_pMyLable[1]->move(500,100);
    m_pMyLable[1]->RemberPos(500,100);
    m_pMyLable[1]->Enable(true);

    m_pMyLable[2] = new myLable(m_pStudentLabelContent);
    m_pMyLable[2]->move(100,400);
    m_pMyLable[2]->RemberPos(100,400);
    m_pMyLable[2]->Enable(false);

    m_pMyLable[3] = new myLable(m_pStudentLabelContent);
    m_pMyLable[3]->move(500,400);
    m_pMyLable[3]->RemberPos(500,400);
    m_pMyLable[3]->Enable(false);

    m_pTeachetVMQWidget = ui->Teacher_scrollAreaWidgetContents;
//    m_pTeacherVMLayOut = new QVBoxLayout(m_pTeachetVMQWidget);
//    m_pTeacherVMGridLayOut = new QGridLayout(m_pTeachetVMQWidget);
    m_pTeachetVMQWidget->setMinimumSize(300,1700);
    m_pTeacherVM[0] = new TeacherVM(m_pTeachetVMQWidget,"close",CLOSE);
    m_pTeacherVM[0]->move(80,10);

    m_pTeacherVM[1] = new TeacherVM(m_pTeachetVMQWidget,"restart",RESTART);
    m_pTeacherVM[1]->move(80,286);

    m_pTeacherVM[2] = new TeacherVM(m_pTeachetVMQWidget,"normal",USING);
    m_pTeacherVM[2]->move(80,562);

    m_pTeacherVM[3] = new TeacherVM(m_pTeachetVMQWidget,"error",ERROR);
    m_pTeacherVM[3]->move(80,838);

    m_pTeacherVM[4] = new TeacherVM(m_pTeachetVMQWidget,"warning",WARNING);
    m_pTeacherVM[4]->move(80,1114);

//    m_pTeacherVMLayOut->addWidget(m_pTeacherVM[0]);
//    m_pTeacherVMLayOut->addWidget(m_pTeacherVM[1]);
//    m_pTeacherVMLayOut->addWidget(m_pTeacherVM[2]);
//    m_pTeacherVMLayOut->addWidget(m_pTeacherVM[3]);
//    m_pTeacherVMLayOut->addWidget(m_pTeacherVM[4]);
//    m_pTeacherVMGridLayOut->addLayout(m_pTeacherVMLayOut,0,0,1,1,Qt::AlignCenter);

//    m_pTeacherVM[5] = new TeacherVM(m_pTeachetVMQWidget,"test");
//    m_pTeacherVM[5]->move(10,1060);

//    m_pTeacherVM[6] = new TeacherVM(m_pTeachetVMQWidget,"test");
//    m_pTeacherVM[6]->move(10,1270);

//    m_pTeacherVM[7] = new TeacherVM(m_pTeachetVMQWidget,"test");
//    m_pTeacherVM[7]->move(10,1480);

    m_pClassNameLabel_1 = ui->ClassNamelabel_1;
    m_pClassNameLabel_1->setPixmap(QPixmap(CLASSNAME1));
    m_pClassNameLabel_2 = ui->ClassNamelabel_2;
    m_pClassNameLabel_2->setPixmap(QPixmap(CLASSNAME2));
    m_pClassNameLabel_3 = ui->ClassNamelabel_3;
    m_pClassNameLabel_3->setPixmap(QPixmap(CLASSNAME1));

}

TeacherForm::~TeacherForm()
{
    delete ui;
}

void TeacherForm::on_BackpushButton_clicked()
{
    this->close();
}
void TeacherForm::SetTeachetName(QString Name)
{
    m_strTeacherName = Name;
    m_pTeacherName->setText(m_strTeacherName);
}
void TeacherForm::SetClassRoom(QString number)
{
    m_strClassRoomNum = number;
    m_pClassRoomNum->setText(m_strClassRoomNum);
}
