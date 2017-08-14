#ifndef CALLNAME_H
#define CALLNAME_H

#include <QWidget>
#include <QString>

namespace Ui {
class CallName;
}

class CallName : public QWidget
{
    Q_OBJECT

public:
    explicit CallName(QWidget *parent = 0);
    ~CallName();
    QString classNum;
    QString name;

private slots:
    void on_pushButtonEnsure_clicked();

private:
    Ui::CallName *ui;
};

#endif // CALLNAME_H
