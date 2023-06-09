#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QItemSelectionModel>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    ui->lineEdit_user->setPlaceholderText("数据库用户名");
    ui->lineEdit_pwd->setPlaceholderText("用户密码");
    ui->lineEdit_table->setPlaceholderText("打开的表名");
    ui->lineEdit_ODBCname->setPlaceholderText("ODBC数据源名称");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_BtnAdd_clicked()
{
    //添加空记录
    QSqlRecord record = model->record();//获取空记录
    //获取行号
    int row = model->rowCount();
    model->insertRecord(row, record);
}

void Widget::on_BtnSure_clicked()
{
    model->submitAll();//提交动作
}

void Widget::on_BtnCancel_clicked()
{
    model->revertAll(); //取消所有动作
    model->submitAll(); //提交所有动作
}

void Widget::on_BtnDel_clicked()
{
    //获取选中的模型
    QItemSelectionModel *sModel = ui->tableView->selectionModel();
    //取出模型中的索引
    QModelIndexList list = sModel->selectedRows();
    //删除所有选中行
    for(int i = 0; i<list.size();i++)
    {
        model->removeRow(list.at(i).row());
    }
}

void Widget::on_BtnFind_clicked()
{
    QString name = ui->lineEdit->text();
    QString str = QString("数量 = '%1'").arg(name);
    model->setFilter(str);
    model->select();
}

void Widget::on_conBtn_clicked()
{
    db.close();
    //载入指定驱动
    db = QSqlDatabase::addDatabase("QODBC");
    //设置数据库名
    db.setHostName("localhost");
    db.setDatabaseName(ui->lineEdit_ODBCname->text());
    //获取数据库用户名和密码
    db.setUserName(ui->lineEdit_user->text());
    db.setPassword(ui->lineEdit_pwd->text());
    //判断是否成功打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,"error",db.lastError().text());
        return;
    }
    //设置数据库数据读取对象
    model = new QSqlTableModel(this);
    //将数据置入布局
    model->setTable(ui->lineEdit_table->text());
    ui->tableView->setModel(model);
    //读取内容
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
