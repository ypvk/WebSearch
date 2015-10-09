#ifndef JOBPARAMDIALOG_H
#define JOBPARAMDIALOG_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class JobParamDialog : public QDialog
{
    Q_OBJECT
public:
    explicit JobParamDialog(QWidget *parent = 0);

    int getClickNum();
    int getThreadNum();

private:
    QLabel * clickNumLabel;
    QLineEdit* clickNum;
    QLabel* threadNumLabel;
    QSpinBox* threadNumValue;
    QPushButton* okButton;
    QPushButton* cancelButton;

};

#endif // JOBPARAMDIALOG_H
