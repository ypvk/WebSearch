#include "jobparamdialog.h"
#include <QtGui>

JobParamDialog::JobParamDialog(QWidget *parent) :
    QDialog(parent)
{
    clickNumLabel = new QLabel(tr("click num"), this);
    clickNum = new QLineEdit(this);
    clickNum->setText("1");
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(clickNumLabel);
    layout->addWidget(clickNum);
    okButton = new QPushButton(tr("OK"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);
    QHBoxLayout* layout1 = new QHBoxLayout;
    layout1->addWidget(okButton);
    layout1->addWidget(cancelButton);

    mainLayout->addLayout(layout);
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
