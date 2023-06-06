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
    //打印Qt支持的数据库驱动
    qDebug()<<QSqlDatabase::drivers();
    //添加Sqlite数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //设置数据库
    QString dsn = QString::fromLocal8Bit("qtcon");
    db.setHostName("localhost");
    db.setDatabaseName(dsn);
    db.setUserName("qtcono");                               //登录用户
    db.setPassword("123456");                              //密码
    //打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,"error",db.lastError().text());
        return;
    }

    //设置模型
    model = new QSqlTableModel(this);
    model->setTable("产品表");//指定使用哪个表
    //把model->放在view
    ui->tableView->setModel(model);
    //显示model的数据
    model->select();
    model->setHeaderData(0,Qt::Horizontal,"产品表");

    //设置model的编辑模式，手动提交修改
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //设置view中的数学库不允许修改
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    QString str = QString("name = '%1'").arg(name);//单引号不能忘
    model->setFilter(str);
    model->select();
}
