#include "jobparamdialog.h"
#include <QtGui>

JobParamDialog::JobParamDialog(QWidget *parent) :
    QDialog(parent)
{
    clickNumLabel = new QLabel(tr("循环次数"), this);
    clickNum = new QLineEdit(this);
    clickNum->setText("1");
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(clickNumLabel);
    layout->addWidget(clickNum);
    okButton = new QPushButton(tr("确定"), this);
    cancelButton = new QPushButton(tr("取消"), this);
    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(okButton);
    layout1->addWidget(cancelButton);

    QHBoxLayout* layout2 = new QHBoxLayout;
    threadNumLabel = new QLabel(tr("线程数"), this);
    threadNumValue = new QSpinBox(this);
    threadNumValue->setMinimum(1);
    threadNumValue->setMaximum(10);
    layout2->addWidget(threadNumLabel);
    layout2->addWidget(threadNumValue);

    mainLayout->addLayout(layout);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout1);

    this->setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

int JobParamDialog::getClickNum()
{
    int num = clickNum->text().toInt();
    if (num >= 0) return num;
    else return 0;
}
int JobParamDialog::getThreadNum()
{
    int num = threadNumValue->value();
    return num;
}
