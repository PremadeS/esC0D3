#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "codeeditor.h"
#include "highlighter.h"
enum Theme
{
    white,
    black
};
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_treeView_clicked(const QModelIndex &index);
    void runFile();     //Execute the the file :)
    void onSaveTriggered();


    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();
    void onTextChanged();
    void newFile();

private:

    void init();
    void setUpSplitter();
    void setUpFontSlider();
    void setUpToolBar();
    void setUpStatusBar();
    void changeTheme();
    void setTreeModel(const QString& folderPath);
    void loadContentsOfFile(const QString& filePath);
    bool saveFile(QString& filePath);
    bool saveFileAs(QString& filePath);
    void executeCommand(const char* command);
    QString getFileName(const QString& filePath);
    void formatCode();
    void checkSyntaxHighLight();

    bool alreadyAdded;
    bool textChanged;
    bool isDefaultFile;

    QSplitter *split;
    Ui::MainWindow *ui;
    QFileSystemModel *treeModel,*fileSystemModel;
    QSlider* fontSizeSlider;
    QToolBar* toolBar;
    QStatusBar *statusBar;
    QString name = "esC0D3";
    QString currFileName;
    QFileInfo currFileInfo;
    bool checkValidFile(const QString& filePath);
    CodeEditor* codeEdit;
    CppSyntaxHighlighter *highLight;
    Theme currTheme = black;
};
#endif // MAINWINDOW_H
