#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlTableModel>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QSqlDatabase db;

private slots:
    void on_BtnAdd_clicked();

    void on_BtnSure_clicked();

    void on_BtnCancel_clicked();

    void on_BtnDel_clicked();

    void on_BtnFind_clicked();


    void on_conBtn_clicked();

private:
    Ui::Widget *ui;
    QSqlTableModel *model;
};

#endif // WIDGET_H
