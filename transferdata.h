#ifndef TRANSFERDATA_H
#define TRANSFERDATA_H

#define RAISE_HAND          1000 //举手命令
#define CANCEL_RAISE_HAND   1001 //取消举手命令
#define FORBID_RAISE_HAND   1002 //禁止举手
#define ALLOW_RAISE_HAND    1003 //允许举手
#define SEND_FILE           1004 //发送文件（学生端/教师端）
#define SET_CLASS_NUM       1005 //设置教室号
#define SET_SEAT_NUM        1006 //设置座位号
#define BLACK_SCREEN        1007 //黑屏
#define CANCEL_BLACK_SCREEN 1008 //取消黑屏
#define STUDENT_EXIT        1009 //学生端退出
#define FORBID_USB_DEVICE   1010 //禁USB存储设备
#define ALLOW_USB_DEVICE    1011 //允许USB存储设备
#define FORBID_NETWORK      1012 //禁止上网
#define ALLOW_NETWORK       1013 //允许上网

int  len;           //数据的长度
int  cmdType;       //命令类型

#endif // TRANSFERDATA_H
