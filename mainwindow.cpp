#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow),textChanged(false),isDefaultFile(true)
{
    //Initializations...
    ui->setupUi(this);

    init();
    setUpSplitter();
    setUpFontSlider();
    setUpToolBar();
    setUpStatusBar();

    // Syntax HighLighter...
    highLight = nullptr;


    connect(codeEdit,&CodeEditor::textChanged,this,&MainWindow::onTextChanged); //Check if the text has changed to give warningof save... :)

    changeTheme();  //Set the initial Theme
}
void MainWindow::init()
{
    setWindowTitle("esC0D3    ( untitled.txt )");
    setWindowIcon(QIcon(":/resources/icon"));
    currFileName = "untitled.txt";

    treeModel = new QFileSystemModel(this);
    fileSystemModel = new QFileSystemModel(this);
    codeEdit = new CodeEditor(this);
    alreadyAdded = false;

    //Font
    QFont textFont("JetBrains Mono Medium",20);
    textFont.setPointSize(15);
    codeEdit->setFont(textFont);

    ui->treeView->setStatusTip("File Explorer");

}
void MainWindow::setUpSplitter()
{
    //Splitter
    split = new QSplitter(this);
    split->addWidget(codeEdit);
    split->addWidget(ui->treeView);    //Set up The splitter in between :)
    split->widget(1)->hide();          // Initialy hide the treeView

    QList<int> sizes;
    sizes << 300 << 100;                //Setting Initial Sizes
    split->setSizes(sizes);

    setCentralWidget(split);

}
void MainWindow::setUpFontSlider()
{
    // Slider for Font Size
    fontSizeSlider = new QSlider(Qt::Horizontal, this);
    fontSizeSlider->setRange(1, 50);
    fontSizeSlider->setValue(15); // Set initial font size.... :)
    fontSizeSlider->setTickInterval(1);
    fontSizeSlider->setMaximumWidth(220);
    fontSizeSlider->setStatusTip("Change Font Size");

    QObject::connect(fontSizeSlider, &QSlider::valueChanged, [&](int value)
    {

     QFont font = codeEdit->font();
     font.setPointSize(value);
     codeEdit->setFont(font);

    });

    //Aesthetic :)
    fontSizeSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "background: transparent;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: lightblue;"
        "   width: 20px;"
        "   margin: -5px 0;"
        "   border-radius: 9px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "   background: gray;"
        "}"
        );

}
void MainWindow::setUpToolBar()
{
    toolBar = new QToolBar("Options");
    addToolBar(Qt::BottomToolBarArea,toolBar);


    QAction *upload = new QAction(this);
    upload->setIcon(QIcon(":/resources/upload.png"));
    upload->setToolTip("Used to Upload an existing File");
    upload->setStatusTip("Upload");
    connect(upload, &QAction::triggered, this, &MainWindow::on_actionOpen_triggered);

    QAction *save = new QAction(this);
    save->setIcon(QIcon(":/resources/save.png"));
    save->setToolTip("Used to save the current file");
    save->setStatusTip("Save");
    connect(save, &QAction::triggered, this, &MainWindow::onSaveTriggered);


    QAction *New = new QAction(this);
    New->setIcon(QIcon(":/resources/new.png"));
    New->setToolTip("Used to make a New File");
    New->setStatusTip("New");
     connect(New, &QAction::triggered, this, &MainWindow::newFile);

    QAction *format = new QAction(this);
    format->setIcon(QIcon(":/resources/format.png"));
    format->setToolTip("Used to format the code | Aesthetic :) | (cpp Only)");
    format->setStatusTip("Format");
    connect(format, &QAction::triggered, this, &MainWindow::formatCode);

    QAction *theme = new QAction(this);
    theme->setIcon(QIcon(":/resources/theme.png"));
    theme->setToolTip("Used to change the Theme");
    theme->setStatusTip("Theme");
    connect(theme, &QAction::triggered, this, &MainWindow::changeTheme);

    QAction *run = new QAction(this);
    run->setIcon(QIcon(":/resources/run.png"));
    run->setToolTip("Used to run the current file (cpp Only)");
    run->setStatusTip("Run");
    connect(run, &QAction::triggered, this, &MainWindow::runFile);

    //Spacer
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);


    //Adding widgets to toolBar :)

    toolBar->addAction(New);
    toolBar->addSeparator();
    toolBar->addSeparator();    //Spaces :)
    toolBar->addSeparator();
    toolBar->addAction(save);
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addAction(upload);
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addAction(format);
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addAction(theme);
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addAction(run);
    toolBar->addWidget(spacer);
    toolBar->addWidget(fontSizeSlider);


}
void MainWindow::setUpStatusBar()
{
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

}

void MainWindow::changeTheme()
{
    if(currTheme==black)    //Toggle between themes :)
    {
        toolBar->setStyleSheet("QToolBar QToolButton { background-color: #f0f0f0; color: black;}"
                               "QToolButton:hover { color: black;}"
                               "QToolBar::separator {background-color: transparent;}");
        statusBar->setStyleSheet("QStatusBar { background-color: #f0f0f0; color: black;}");
        ui->menubar->setStyleSheet("QMenuBar { background-color: #f0f0f0; color: black;}");
        ui->treeView->setStyleSheet("QTreeView { background-color: #f0f0f0; color: black;}");       //Light Theme
        setStyleSheet("background-color: #f0f0f0; color: black");
        codeEdit->setStyleSheet("background-color: white; color:#444444");
        currTheme = white;
    }
    else
    {
        toolBar->setStyleSheet("QToolBar QToolButton { background-color: #1e1e1e; color: white;}"
                               "QToolButton:hover { color: gray;}"
                               "QToolBar::separator {background-color: transparent;}");
        statusBar->setStyleSheet("QStatusBar { background-color: #1e1e1e; color: white;}");     //Dark Theme
        ui->menubar->setStyleSheet("QMenuBar { background-color: #1e1e1e; color: white;}");
        ui->treeView->setStyleSheet("QTreeView { background-color: #1e1e1e; color: white;}");
        setStyleSheet("background-color: #1e1e1e; color: white");
        codeEdit->setStyleSheet("background-color: #2d2d2d; color:lightgray");
        currTheme = black;
    }
}

void MainWindow::onSaveTriggered()
{
    QString filePath = currFileInfo.absoluteFilePath();
    saveFile(filePath);
}
bool MainWindow::saveFileAs(QString& filePath)
{
    QString filter = "Text Files (*.txt);;HTML Files (*.html);;C++ Files (*.cpp);;Python Files (*.py);;C# Files (*.cs);;All Files (*)";
    filePath = QFileDialog::getSaveFileName(nullptr, "Save File", QDir::homePath() + "/" + currFileName, filter);
    if (filePath.isEmpty()) {
        return false; // User cancelled operation...
    }
    currFileInfo = QFileInfo(filePath);
    currFileName = currFileInfo.fileName();
    setWindowTitle(name + "   (" +currFileName+")");
    if(!alreadyAdded&&!filePath.isEmpty())
    {
        setTreeModel(currFileInfo.absolutePath());
        split->widget(1)->show();
        alreadyAdded = true;            //Only want to show the treeView once
    }

    QString content= codeEdit->toPlainText();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Could not save file.");
        return false;
    }
    QTextStream out(&file);
    out << content;
    textChanged = false;
    isDefaultFile = false;
    checkSyntaxHighLight();
    return true;
}
void MainWindow::checkSyntaxHighLight()
{
    if(currFileInfo.suffix()=="cpp"&&highLight==nullptr)
    {
        highLight = new CppSyntaxHighlighter(codeEdit->document());
    }
    else if(currFileInfo.suffix()!="cpp"&&highLight != nullptr)
    {
        delete highLight;
        highLight = nullptr;
    }
}
bool MainWindow::saveFile(QString& filePath) {

    bool savedAs = false;

    QString content= codeEdit->toPlainText();
    if (filePath.isEmpty())
    {
        if(!saveFileAs(filePath))
            return false;
        else
            savedAs = true;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Could not save file.");
        return false;
    }

    if(!savedAs)
    {
        QTextStream out(&file);
        out << content;
        currFileInfo = QFileInfo(filePath);
        currFileName = currFileInfo.fileName();
        setWindowTitle(name + "   (" +currFileName+")");
        if(!alreadyAdded&&!filePath.isEmpty())
        {
            setTreeModel(currFileInfo.absolutePath());
            split->widget(1)->show();
            alreadyAdded = true;            //Only want to show the treeView once
        }
    }
    file.close();
    checkSyntaxHighLight();
    textChanged = false;
    isDefaultFile = false;
    return true;
}
void MainWindow::on_actionSave_triggered()
{
    QString path(currFileInfo.absoluteFilePath());
    saveFile(path);
}
void MainWindow::on_actionSave_As_triggered()
{
    QString path(currFileInfo.absoluteFilePath());
    saveFileAs(path);
}


void MainWindow::on_actionOpen_triggered()
{
    QString filter = "All Supported Files (*.cpp *.txt *.py *.html)";
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", "", filter);
    QFileInfo fileInfo(filePath);
    QString folderPath = fileInfo.absolutePath();


    if(!filePath.isEmpty())
    {
        setTreeModel(folderPath);
        loadContentsOfFile(filePath);
    }
    if(!alreadyAdded&&!filePath.isEmpty())
    {
        split->widget(1)->show();
        alreadyAdded = true;            //Only want to show the treeView once
    }
    isDefaultFile = false;
    checkSyntaxHighLight();

}



bool MainWindow::checkValidFile(const QString& filePath)
{
    QFileInfo info(filePath);
    QString extension = info.suffix();
    QStringList validFiles;
    validFiles << "txt" << "cpp" <<"html"<<"py";
    for(auto v : validFiles)
    {
        if(v == extension)
            return true;
    }
    return false;
}
void MainWindow::loadContentsOfFile(const QString& filePath)
{
    if(checkValidFile(filePath))
    {
        currFileInfo = QFileInfo(filePath);
        currFileName = getFileName(filePath);
        setWindowTitle(name + "   (" +currFileName+")");

        if(!filePath.isEmpty())
        {

            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString contents = file.readAll();
                codeEdit->setPlainText(contents);
                file.close();
            }

        }
        else
            codeEdit->setPlainText("Failed to Open file :(");
    }
    else
        codeEdit->setPlainText("Unsupported Format");
}
QString MainWindow::getFileName(const QString& filePath)
{
    QFileInfo info(filePath);
    return info.fileName();
}
void MainWindow::runFile()
{
    QString path(currFileInfo.absoluteFilePath());
    if(!path.isEmpty()&&currFileInfo.suffix()=="cpp")
    {
        QString run("cmd.exe /K \"g++ "+currFileInfo.absoluteFilePath()+" && a\"");
        executeCommand(run.toUtf8().constData());
    }
}
void MainWindow::executeCommand(const char* command) {
    std::thread([command]() {
        system(command);
    }).detach();
}

void MainWindow::newFile()
{
    if(highLight!=nullptr)
        delete highLight;
    if (textChanged)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Confirm Exit");

        // Custom icon :)
        QPixmap iconPixmap(":/resources/warning_icon.png");
        msgBox.setIconPixmap(iconPixmap);

        msgBox.setText("You have unsaved changes. Do you want to save before exiting?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int resBtn = msgBox.exec(); // What da user clicked bruv...

        if (resBtn == QMessageBox::Cancel)
        {
            return;
        }
        else if (resBtn == QMessageBox::Yes) {
            QString path =  currFileInfo.absoluteFilePath();
            saveFile(path);
        }

    }
    if(!isDefaultFile)
    {
        codeEdit->setPlainText("");
        currFileName = "untitled.txt";
        currFileInfo.setFile("");
        setWindowTitle(name + " ( "+currFileName+" )");
        isDefaultFile = true;
        textChanged = false;
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString filePath=fileSystemModel->filePath(index);
    loadContentsOfFile(filePath);
    checkSyntaxHighLight();
}
void MainWindow::setTreeModel(const QString& folderPath)
{
    treeModel->setRootPath(folderPath);
    ui->treeView->setModel(treeModel);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);          //Only show name and hide other stuff
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex(treeModel->index(folderPath));

}

void MainWindow::formatCode()
{
    codeEdit->formatCode();
}

void MainWindow::keyPressEvent(QKeyEvent *event)    // ShortCuts
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Equal) //ctrl +
    {
        int newValue = fontSizeSlider->value() + 1;
        fontSizeSlider->setValue(newValue);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus) //ctrl -
    {
        int newValue = fontSizeSlider->value() - 1;
        fontSizeSlider->setValue(newValue);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_O) //ctrl + O
    {
        on_actionOpen_triggered();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_T) //ctrl + t
    {
        changeTheme();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {
        codeEdit->formatCode();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
    {
        QString path(currFileInfo.absoluteFilePath());
        saveFile(path);
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_R)
    {
        runFile();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
void MainWindow::onTextChanged()
{
    textChanged = true;
    isDefaultFile = false;  // The file isnt the default one anymore :(
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (textChanged)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Confirm Exit");

        // Custom icon :)
        QPixmap iconPixmap(":/resources/warning_icon.png");
        msgBox.setIconPixmap(iconPixmap);

        msgBox.setText("You have unsaved changes. Do you want to save before exiting?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int resBtn = msgBox.exec(); // What da user clicked bruv...

        if (resBtn == QMessageBox::Cancel) {
            event->ignore();
            return;
        } else if (resBtn == QMessageBox::Yes) {
            QString path =  currFileInfo.absoluteFilePath();
            saveFile(path);
        }
    }
    event->accept();
}
void MainWindow::on_actionNew_triggered()
{
    newFile();
}


MainWindow::~MainWindow()
{
    delete ui;
}





