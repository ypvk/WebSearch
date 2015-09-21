#include "browser.h"
#include <QtGui>
#include <QtWebKit>

Browser::Browser(QWidget *parent) :
    QMainWindow(parent)
{
        QFile file;
        file.setFileName(":/jquery.min.js");
        file.open(QIODevice::ReadOnly);
        jQuery = file.readAll();
        file.close();
    progress = 0;

       QNetworkProxyFactory::setUseSystemConfiguration(true);

       view = new QWebView(this);
       view->load(QUrl("http://www.baidu.com"));
       connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
       connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
       connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
       connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

       locationEdit = new QLineEdit(this);
       locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
       connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
       QToolBar *toolBar = addToolBar(tr("Navigation"));
       toolBar->addAction(view->pageAction(QWebPage::Back));
       toolBar->addAction(view->pageAction(QWebPage::Forward));
       toolBar->addAction(view->pageAction(QWebPage::Reload));
       toolBar->addAction(view->pageAction(QWebPage::Stop));
       toolBar->addWidget(locationEdit);

         QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
         QAction* viewSourceAction = new QAction("Page Source", this);
         connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
         viewMenu->addAction(viewSourceAction);

         QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
         effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

         rotateAction = new QAction(this);
         rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
         rotateAction->setCheckable(true);
         rotateAction->setText(tr("Turn images upside down"));
          connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
          effectMenu->addAction(rotateAction);

          QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
          toolsMenu->addAction(tr("Remove GIF images"), this, SLOT(removeGifImages()));
          toolsMenu->addAction(tr("Remove all inline frames"), this, SLOT(removeInlineFrames()));
          toolsMenu->addAction(tr("Remove all object elements"), this, SLOT(removeObjectElements()));
          toolsMenu->addAction(tr("Remove all embedded elements"), this, SLOT(removeEmbeddedElements()));

          setCentralWidget(view);
          setUnifiedTitleAndToolBarOnMac(true);
}
void Browser::viewSource()
{
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(view->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

void Browser::slotSourceDownloaded()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->show();
    textEdit->setPlainText(reply->readAll());
    reply->deleteLater();
}

void Browser::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void Browser::changeLocation()
{
    QUrl url = QUrl(locationEdit->text());
    view->load(url);
    view->setFocus();
}
void Browser::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void Browser::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void Browser::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
    view->page()->mainFrame()->evaluateJavaScript("$('input#kw').val('12231')");
   // view->page()->mainFrame()->evaluateJavaScript(jQuery);

    rotateImages(rotateAction->isChecked());
    view->page()->mainFrame()->evaluateJavaScript("alert(document.getElementById('dw').value)");
    view->page()->mainFrame()->evaluateJavaScript("$('#su').click()");
}
void Browser::highlightAllLinks()
 {
     QString code = "$('a').each( function () { $(this).css('background-color', 'yellow') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void Browser::rotateImages(bool invert)
 {
     QString code;
     if (invert)
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(180deg)') } )";
     else
         code = "$('img').each( function () { $(this).css('-webkit-transition', '-webkit-transform 2s'); $(this).css('-webkit-transform', 'rotate(0deg)') } )";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }

 void Browser::removeGifImages()
 {
     QString code = "$('[src*=gif]').remove()";
     view->page()->mainFrame()->evaluateJavaScript(code);
 }
 void Browser::removeInlineFrames()
  {
      QString code = "$('iframe').remove()";
      view->page()->mainFrame()->evaluateJavaScript(code);
  }

  void Browser::removeObjectElements()
  {
      QString code = "$('object').remove()";
      view->page()->mainFrame()->evaluateJavaScript(code);
  }

  void Browser::removeEmbeddedElements()
  {
      QString code = "$('embed').remove()";
      view->page()->mainFrame()->evaluateJavaScript(code);
  }
