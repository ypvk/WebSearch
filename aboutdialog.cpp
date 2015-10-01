#include "aboutdialog.h"
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    content = new QTextEdit(this);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(content);
    this->setLayout(layout);
    this->setWindowTitle("About");
    content->append("========about===========\n");
    content->append("1. config action will show configDialog\n");
    content->append(" * click config action, show config dialog\n");
    content->append(" * click load button to load file from outside, file format(txt):\n");
    content->append("     *  proxy: \"10.10.10.5 9090\"\n");
    content->append("     *  engine: \"百度 http://www.baidu.com\"\n");
    content->append("     *  keyword: \"musper\"\n");
    content->append(" * click delete selected button can delete the item selected on the table view\n");
    content->append(" * click submit button can update the changed value into db(double click the table item can edit it)\n");
    content->append("2. startJob action will start search job\n");
    content->append("3. stopJob action will stop search job(only avaliable when job is running\n");
    content->append("4. mainwindow\n");
    content->append(" * realtime info will show every search info detail\n");
    content->append(" * click info will show the key word click times\n");
    this->resize(500, 400);
}
