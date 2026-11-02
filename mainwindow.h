#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_action_O_triggered();

    void on_actionSave_triggered();

    void on_actionsSaveAs_triggered();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);


    void on_textEdit_textChanged();

    void on_actionFont_triggered();

    void on_actionAutoWrap_triggered();

    void on_actionFontColor_triggered();

    void on_actionEditBackColor_triggered();

    void on_actionFontBackColor_triggered();

    void on_actionToolbar_triggered();

    void on_actionStatusBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_textEdit_cursorPositionChanged();

    //void on_actionShowLine_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;
    bool maybeSave();
    void setWindowTitleByPath();
    bool isModified;
    QString currentFilePath;
};
#endif // MAINWINDOW_H
