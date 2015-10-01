#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QTextEdit;

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget* parent = 0);
private:
    QTextEdit* content;
};

#endif // ABOUTDIALOG_H
