#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    setParent(parent);
    pFileDialog = new QFileDialog;
    setWindowTitle(tr("DOM XML Parser"));
    const QSize availableSize = QApplication::desktop()->availableGeometry(this).size();
    setMinimumSize(availableSize.width() / 3, availableSize.height() / 2);

    createMenus();
    xQueryWidget = createXQueryWidget();

    xQueryWidget->setMaximumHeight(availableSize.height()/5);
    connect(loadXqPb, SIGNAL(clicked()), this, SLOT(openXQ()));
    connect(startXqPb, SIGNAL(clicked()), this, SLOT(startXQ()));

    pTreeView = new QTreeView(nullptr);
    pTreeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    //pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pTreeView->setSelectionMode(QAbstractItemView::NoSelection);
    myDelegate *delegate = new myDelegate (this);
    pTreeView->setItemDelegate(delegate); //установка делегата

    pMainLayout = new QGridLayout;
    pMainLayout->setContentsMargins(0,0,0,0);
    pMainWidget = new QWidget;
    pMainLayout->addWidget(pTreeView,0,0,2,1);
    pMainLayout->addWidget(xQueryWidget,2,0,1,1);
    pMainWidget->setLayout(pMainLayout);
    setCentralWidget(pMainWidget);

    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Menu"));
    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAct->setShortcuts(QKeySequence::Open);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::Save);

    fileMenu->addAction(tr("&Close All"), this, &MainWindow::closeAll, Qt::CTRL + Qt::Key_F);

    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Close);
}

void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = pTreeView->currentIndex().data().tos
    qDebug() << index.row();
    if (!index.isValid() || index.parent().isValid())
        return;

    if (!contextMenu)
    {
        contextMenu = new QMenu;
        removeAction = new QAction("Delete last", contextMenu);
        contextMenu->addAction(removeAction);
    }

    QPoint globalPos = pTreeView->mapToGlobal(point);
    QAction* selectedItem = contextMenu->exec(globalPos);
    if (selectedItem == removeAction)
    {
        if (domModel->remove_s(index) == -1)
        {
            domModel = nullptr;
            singleXMLPath = "null";
        }
        contextMenu->hide();
    }
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open XML File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("DOM Parser"), tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    if (!domModel)
    {
        domModel = new DomModel(this);
        if (domModel->read(&file))
        {
            statusBar()->showMessage(tr("File loaded"), 3000);
            singleXMLPath = fileName;
        }
        file.close();
        pTreeView->setModel(domModel);
        setWindowTitle(QFileInfo(fileName).fileName() + " - DOM XML Parser");
    }
    else
    {
        QFile appFile(fileName);
        if (domModel->add(&appFile))
        {
            statusBar()->showMessage(tr("File appended"), 3000);
            singleXMLPath = "";
        }
        appFile.close();
    }
}

void MainWindow::saveAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save XML File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DOM Parser"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }

    if (domModel->write(&file))
        statusBar()->showMessage(tr("File saved"), 2000);
    file.close();
}

void MainWindow::closeAll()
{
    if (domModel)
    {
        delete domModel;
        statusBar()->showMessage(tr("Successfully deleted"), 3000);
        domModel = nullptr;
        singleXMLPath = "null";
    }
}

QWidget * MainWindow::createXQueryWidget()
{
    QWidget *xqWidget = new QWidget;
    QGridLayout *xqLayout = new QGridLayout;
    xqTextEdit = new QTextEdit;
    xqTextEdit->setStyleSheet("QTextEdit{"
                              "    font-size: 20px;}");

    loadXqPb = new MyPushButton("Load file");
    startXqPb = new MyPushButton("Start");
    loadXqPb->setMaximumWidth(300);
    startXqPb->setMaximumWidth(300);

    loadXqPb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    startXqPb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    xqLayout->setContentsMargins(0,0,0,0);

    xqLayout->addWidget(xqTextEdit, 0,0, 2, 1);
    xqLayout->addWidget(loadXqPb, 0, 1, 1, 1);
    xqLayout->addWidget(startXqPb, 1, 1, 1, 1);

    xqWidget->setLayout(xqLayout);

    pResultText = new QTextEdit;
    pResultText->setStyleSheet("QTextEdit{"
                               "    font-size: 20px;}");
    pResultText->resize(500, 400);
    return xqWidget;
};

QString MainWindow::getXMLFile()
{
    QString xmlFileName = singleXMLPath;
    if (xmlFileName == "null")
    {
        statusBar()->showMessage(tr("XML file does not loaded yet."), 3000);
        return "failed";
    }
    if (xmlFileName == "")
    {
        //save temp xml tree
        xmlFileName = QFileDialog::getSaveFileName(this, tr("Save XML File"),
                                               QDir::currentPath(),
                                               tr("XML Files (*.xml)"));
        QFile xmlFile(xmlFileName);
        if (!xmlFile.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("DOM Parser"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(xmlFileName),
                                      xmlFile.errorString()));
            return "failed";
        }
        domModel->write(&xmlFile);
        xmlFile.close();
        singleXMLPath = xmlFileName;
    }
    return xmlFileName;
}

void MainWindow::openXQ()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XQ File"),
                                         QDir::currentPath(),
                                         tr("XQ Files (*.xq)"));
    QFile xqFile(fileName);
    QString strQuery;
    if (xqFile.open(QFile::ReadOnly))
    {
        QFile xmlFile;
        QString xmlFileName = getXMLFile();
        if (xmlFileName == "failed") return;

        strQuery = xqFile.readAll();
        emit setText(strQuery);
        xqFile.close();

        //execXQ(xmlFileName);
    }
    else
    {
        QMessageBox::warning(this, tr("DOM Parser"), tr("Cannot write file."));
        return;
    }
}

void MainWindow::startXQ()
{
    QFile xmlFile;
    QString xmlFileName = getXMLFile();
    if (xmlFileName == "failed") return;

    execXQ(xmlFileName);
}


void MainWindow::execXQ(const QString &xmlPath)
{
    QString strQuery = xqTextEdit->toPlainText();
    //open temp xml file and app xq
    QFile xmlFile;
    xmlFile.setFileName(xmlPath);
    if (xmlFile.open(QIODevice::ReadOnly))
    {
        QXmlQuery query;
        query.bindVariable("inputDocument", &xmlFile);
        query.setQuery(strQuery);
        if (!query.isValid())
        {
            statusBar()->showMessage(tr("The query is invalid"), 3000);
            return;
        }

        QString strOutput;
        if (!query.evaluateTo(&strOutput))
        {
            statusBar()->showMessage(tr("Can't evaluate the query"), 3000);
            xmlFile.close();
            return;
        }

        xmlFile.close();

        pResultText->setText(strOutput);
        pResultText->show();
        pResultText->activateWindow();
    }
}

void MainWindow::setText(const QString & strQuery)
{
    xqTextEdit->setPlainText(strQuery);
}
