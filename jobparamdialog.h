#ifndef JOBPARAMDIALOG_H
#define JOBPARAMDIALOG_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;

class JobParamDialog : public QDialog
{
    Q_OBJECT
public:
    explicit JobParamDialog(QWidget *parent = 0);

    int getClickNum();

private:
    QLabel * clickNumLabel;
    QLineEdit* clickNum;
    QPushButton* okButton;
    QPushButton* cancelButton;

};

#endif // JOBPARAMDIALOG_H
