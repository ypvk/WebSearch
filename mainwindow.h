#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class QWebView;
class QLineEdit;
class QPushButton;
class QUrl;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void onButtonClicked();
    void onPageLoadFinished();
    void onUrlChanged(const QUrl& url);
private:
    void setupGui();
    void setupAction();
private:
    Ui::MainWindow *ui;
    QWebView* mWebView;
    QLineEdit* mLineEdit;
    QPushButton* mPushButton;
    QWidget* mMainWidget;

};

#endif // MAINWINDOW_H
