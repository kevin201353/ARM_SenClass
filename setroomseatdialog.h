#ifndef SETROOMSEATDIALOG_H
#define SETROOMSEATDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFont>

namespace Ui {
class SetRoomSeatDialog;
}

class SetRoomSeatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetRoomSeatDialog(QWidget *parent = 0);
    ~SetRoomSeatDialog();
    void SetRoomSeatText(char *Room,char*Seat);
private slots:
    void on_OKpushButton_clicked();

    void on_CancelpushButton_clicked();

    void on_RoomNumlineEdit_textChanged(const QString &arg1);

    void on_SeatNumlineEdit_textChanged(const QString &arg1);

private:
    Ui::SetRoomSeatDialog *ui;
    QPushButton *m_pOKPushButton;
    QPushButton *m_pCancelPushButton;
    QLineEdit   *m_pRoomNumLineEdit;
    QLineEdit   *m_pSeatNumLineEdit;
    QLabel      *m_pRoomNumLabel;
    QLabel      *m_pSeatNumLabel;
    QFont       m_Font;
};

#endif // SETROOMSEATDIALOG_H
