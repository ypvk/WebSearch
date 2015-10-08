#include "aboutdialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    content = new QTextEdit(this);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(content);
    this->setLayout(layout);
    this->setWindowTitle("关于");
    content->append("========关于===========\n");
    content->append("1. 点击 配置 action 讲话弹出配置窗口\n");
    content->append(" * 点击 导入 按钮 会导入外部文件, 文件格式(.txt):\n");
    content->append("     *  代理: \"10.10.10.5 9090\"或\"10.10.10.5:9090\"\n");
    content->append("     *  搜索引擎: \"百度 http://www.baidu.com\"\n");
    content->append("     *  关键词: \"musper\"\n");
    content->append(" * 点击 删除选中 按钮可以删除表格中选中的行\n");
    content->append(" * 点击 提交更改 按钮可以将更改的数据同步到数据库中(双击表格单元可以进行)\n");
    content->append("2. 点击 开始任务 action 可以开始搜索任务，会弹出搜索框，设置点击次数\n");
    content->append("3. 点击 结束任务 action 将会停止搜索任务（只有在任务执行时才有效）\n");
    content->append("4. 主窗口信息\n");
    content->append(" * 实时信息 会显示每次点击的信息（包括：搜索引擎，ip，链接）\n");
    content->append(" * 点击信息会显示每个关键词的点击次数\n");
    this->resize(500, 400);
}
