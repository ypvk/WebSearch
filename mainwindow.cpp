#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QString>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGui();
    setupAction();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupGui()
{
    mMainWidget = new QWidget(this);
    mLineEdit = new QLineEdit(this);
    mPushButton = new QPushButton(tr("enter"), this);
    mWebView = new QWebView(this);

    QHBoxLayout* layout1 = new QHBoxLayout();
    QVBoxLayout* layout2 = new QVBoxLayout(mMainWidget);

    layout1->addWidget(mLineEdit);
    layout1->addWidget(mPushButton);

    layout2->addLayout(layout1);
    layout2->addWidget(mWebView);

    mMainWidget->setLayout(layout1);
    this->setCentralWidget(mMainWidget);
}

void MainWindow::setupAction()
{
    connect(mPushButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    connect(mWebView, SIGNAL(loadFinished(bool)), this, SLOT(onPageLoadFinished()));
    connect(mWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(onUrlChanged(QUrl)));
}

void MainWindow::onButtonClicked()
{
    const QString url = mLineEdit->text();
    mWebView->load(QUrl(url));

}
void MainWindow::onPageLoadFinished()
{
    mWebView->page()->mainFrame()->evaluateJavaScript("$('#kw').val('12231')");
}
void MainWindow::onUrlChanged(const QUrl& url)
{
   mLineEdit->setText(url.toString());
   QMessageBox::information(this, "show url", url.toString());
}
