#include <QtGui>
#include <QFileSystemModel>
#include <QDir>
#include <QStringListModel>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QFormLayout>

#include "editor.h"
#include "mainwindow.h"

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString str = model->filePath(index);
    QFile file(model->filePath(index));
    if(file.open(QFile::ReadOnly | QFile::Text))
        openFile(file.fileName());
}

MainWindow::MainWindow()
{
    QDockWidget *dock = new QDockWidget;
    dock->resize(400, 700);
    dock->setMaximumHeight(10000);
    model = new QFileSystemModel;
    QTreeView *treeView = new QTreeView;
    treeView->setModel(model);

    connect(treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_treeView_doubleClicked(const QModelIndex &)));
    model->setRootPath(QDir::currentPath());
    treeView->setRootIndex(model->index(QDir::homePath()));
    dock->setFloating(true);
    treeView->setEditTriggers( QAbstractItemView::NoEditTriggers);
    dock->setWidget(treeView);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock);

    mdiArea = new QMdiArea;
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setDocumentMode(true);
    mdiArea->setTabsMovable(true);
    mdiArea->setTabsClosable(true);
    mdiArea->setTabShape(QTabWidget::Triangular);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateActions()));

    createActions();
    createMenus();
    createToolBars();

    currentTextEdit = new Editor(this);
    currentTextEdit->hide();

    connect(currentTextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(currentTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    createStatusBar();

    setWindowTitle(tr("MDI Editor"));
    setWindowIcon(QIcon(":/images/blog.png"));
    QTimer::singleShot(0, this, SLOT(loadFiles()));
}

void MainWindow::loadFiles()
{
    QStringList args = QApplication::arguments();
    args.removeFirst();
    if (!args.isEmpty()) {
        foreach (QString arg, args)
            openFile(arg);
        mdiArea->cascadeSubWindows();
    } else {
        newFile();
    }
    mdiArea->activateNextSubWindow();
}

void MainWindow::newFile()
{
    Editor *editor = new Editor;
    editor->newFile();
    QString str = "txt";
    addEditor(editor, str);
}

void MainWindow::openFile(const QString &fileName)
{
    Editor *editor = Editor::openFile(fileName, this);
    QFile file(fileName);
    QByteArray data = file.readAll();
    QString str;
    str = QString::fromLocal8Bit(data);
    currentTextEdit->setPlainText(str);

    QFileInfo fi (fileName);
    QString strg = fi.suffix();
    if (editor)
        addEditor(editor, strg);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (!mdiArea->subWindowList().isEmpty()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::open()
{
    QString name;
    Editor *editor = Editor::open(this, std::move(name));
    QFileInfo fi(name);
    QString str = fi.suffix();

     if (editor)
        addEditor(editor, str);
}

void MainWindow::save()
{
    if (activeEditor())
        activeEditor()->save();
}

void MainWindow::saveAs()
{
    if (activeEditor())
        activeEditor()->saveAs();
}

void MainWindow::cut()
{
    if (activeEditor())
        activeEditor()->cut();
}

void MainWindow::copy()
{
    if (activeEditor())
        activeEditor()->copy();
}

void MainWindow::paste()
{
    if (activeEditor())
        activeEditor()->paste();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MDI Editor"),
            tr("<h2>Editor 1.1</h2>"
               "<p>Copyright &copy;"
               "<p>MDI Editor is a small application that demonstrates "
               "QMdiArea."));
}

void MainWindow::updateActions()
{
    bool hasEditor = (activeEditor() != 0);
    bool hasSelection = activeEditor()
                        && activeEditor()->textCursor().hasSelection();

    saveAction->setEnabled(hasEditor);
    saveAsAction->setEnabled(hasEditor);
    cutAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    pasteAction->setEnabled(hasEditor);
    closeAction->setEnabled(hasEditor);
    closeAllAction->setEnabled(hasEditor);
    tileAction->setEnabled(hasEditor);
    cascadeAction->setEnabled(hasEditor);
    nextAction->setEnabled(hasEditor);
    previousAction->setEnabled(hasEditor);
    separatorAction->setVisible(hasEditor);

    if (activeEditor())
        activeEditor()->windowMenuAction()->setChecked(true);
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the file to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setStatusTip(tr("Save the file under a new name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut the current selection to the "
                               "clipboard"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy the current selection to the "
                                "clipboard"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents at "
                                 "the cursor position"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    closeAction = new QAction(tr("Cl&ose"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close the active window"));
    connect(closeAction, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAction = new QAction(tr("Close &All"), this);
    closeAllAction->setStatusTip(tr("Close all the windows"));
    connect(closeAllAction, SIGNAL(triggered()), this, SLOT(close()));

    tileAction = new QAction(tr("&Tile"), this);
    tileAction->setStatusTip(tr("Tile the windows"));
    connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAction = new QAction(tr("&Cascade"), this);
    cascadeAction->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAction = new QAction(tr("Ne&xt"), this);
    nextAction->setShortcut(QKeySequence::NextChild);
    nextAction->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAction, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    previousAction = new QAction(tr("Pre&vious"), this);
    previousAction->setShortcut(QKeySequence::PreviousChild);
    previousAction->setStatusTip(tr("Move the focus to the previous "
                                    "window"));
    connect(previousAction, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    windowActionGroup = new QActionGroup(this);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(closeAction);
    windowMenu->addAction(closeAllAction);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAction);
    windowMenu->addAction(cascadeAction);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAction);
    windowMenu->addAction(previousAction);
    windowMenu->addAction(separatorAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
}

void MainWindow::createStatusBar()
{
    readyLabel = new QLabel(tr(" Ready"));
    statusBar()->addWidget(readyLabel, 1);
}

void traverseNode(const QDomNode& node, QString& b)
{
    QDomNode domNode = node.firstChild();
    while ( ! domNode. isNull () ) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if ( ! domElement. isNull () ) {
                if(domElement.tagName() == "keywords"){
                        b = domElement.attribute("exp");
                        break;
                }
            }
        }
        traverseNode(domNode, b);
        domNode = domNode.nextSibling();
    }
}

void MainWindow::addEditor(Editor *editor, QString & str)
{   
    connect(editor, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

    QDomDocument domDoc;
    QString Word;
    QFile file ( ":/xml_words.xml") ;
    if(file.open(QIODevice::ReadOnly)) {
        if(domDoc.setContent(&file)) {
            QDomElement domElement= domDoc.documentElement();
            traverseNode(domElement, Word);
        }
        file. close () ;
    }
    QStringList ls = Word.split(" ");
    if(ls[1]==str || ls[2] == str)
        setupEditor(editor);

    QMdiSubWindow *subWindow = mdiArea->addSubWindow(editor);
    windowMenu->addAction(editor->windowMenuAction());
    windowActionGroup->addAction(editor->windowMenuAction());
    subWindow->show();
}

Editor *MainWindow::activeEditor()
{
    QMdiSubWindow *subWindow = mdiArea->activeSubWindow();
    if (subWindow)
        return qobject_cast<Editor *>(subWindow->widget());
    return 0;
}

void MainWindow::setupEditor(Editor *editor)
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}
