#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    isModified = false;
    currentFilePath = "";

    on_actionNew_triggered();

    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
        if (mode == QPlainTextEdit::NoWrap)
        {
            ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
            ui->actionAutoWrap->setChecked(true);
        }
        else
        {
            ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
            ui->actionAutoWrap->setChecked(false);
        }


     ui->actionToolbar->setChecked(true);
     ui->actionStatusBar->setChecked(true);

    statusLabel.setMaximumWidth(1000);
    statusLabel.setText("length: " + QString::number(0) + "    lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(900);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("黄德民"));
    ui->statusbar->addPermanentWidget(author);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//对话框
void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dio;
    dio.exec();
}


void MainWindow::on_actionFind_triggered()
{
    FindDialog dio(this,ui->textEdit);
    dio.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dio(this,ui->textEdit);
    dio.exec();
}


//槽函数
void MainWindow::on_actionNew_triggered()
{
    // 第一步先判断是否需要保存
        if(!maybeSave())
        {
            return;
        }
        ui->textEdit->clear();
        currentFilePath = "";
        isModified = false; // 新建清空，重置修改标记
        setWindowTitleByPath();
}


void MainWindow::on_action_O_triggered()
{
    if(!maybeSave())
        {
            return;
        }
        QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".",
                                                        tr("Text files (*.txt);; All (*.*)"));
        if(filename.isEmpty())
        {
            return;
        }
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("警告"), tr("打开文件失败"));
            return;
        }
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setPlainText(text);
        file.close();

        currentFilePath = filename;
        isModified = false; // 打开文件，重置修改标记
        setWindowTitleByPath();
}


void MainWindow::on_actionSave_triggered()
{
    if(currentFilePath.isEmpty())
        {
            // 没有文件路径，跳另存为
            on_actionsSaveAs_triggered();
            return;
        }
        QFile file(currentFilePath);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("警告"), tr("保存文件失败"));
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();

        isModified = false; //
        setWindowTitleByPath();
}


void MainWindow::on_actionsSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("另存为"), "./untitled.txt",
                                                        tr("Text files (*.txt)"));
        if(filename.isEmpty())
        {
            // 用户弹窗点取消，直接退出，isModified保持原样，标题*保留
            return;
        }
        QFile file(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("警告"), tr("另存为失败"));
            return;
        }
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();

        currentFilePath = filename;
        isModified = false;
        setWindowTitleByPath();
}

bool MainWindow::maybeSave()
{
    if(!isModified)
        {
            // 没有改动，不需要保存，直接放行
            return true;
        }
        QMessageBox::StandardButton ret = QMessageBox::question(this,
                                                                 tr("提示"),
                                                                 tr("文件已经修改，是否保存更改？"),
                                                                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            // 执行保存
            on_actionSave_triggered();
            // 保存之后再判断：如果保存完还是修改状态（比如用户另存弹窗点取消），不能放行
            return !isModified;
        }
        else if(ret == QMessageBox::No)
        {
            // 不保存，直接放行
            return true;
        }
        else
        {
            // 用户取消，阻断后续操作
            return false;
        }


}

void MainWindow::setWindowTitleByPath()
{
    QString title;
        if(currentFilePath.isEmpty())
        {
            title = tr("新建文本文件 - 编辑器");
        }
        else
        {
            title = QFileInfo(currentFilePath).absoluteFilePath();
        }
        if(isModified)
        {
            title = "* " + title;
        }
        this->setWindowTitle(title);
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);

}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_textEdit_textChanged()
{
    if(!isModified)
        {
            isModified = true;
            setWindowTitleByPath();
        }
    statusLabel.setText("length: " + QString::number(ui->textEdit->toPlainText().length()) +
                            "  lines: " + QString::number(ui->textEdit->document()->lineCount()));
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
        QFont font = QFontDialog::getFont(&ok, this);
        if (ok)
        {
            ui->textEdit->setFont(font);
        }
}


void MainWindow::on_actionAutoWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
        if (mode == QPlainTextEdit::NoWrap)
        {
            ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
            ui->actionAutoWrap->setChecked(true);
        }
        else
        {
            ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
            ui->actionAutoWrap->setChecked(false);
        }
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
        if (color.isValid())
        {
            ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color: %1;}").arg(color.name()));
        }
}


void MainWindow::on_actionEditBackColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择背景颜色");
        if (color.isValid())
        {
            ui->textEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1;}").arg(color.name()));
        }
}


void MainWindow::on_actionFontBackColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this, "选择文字背景色");
        if (!color.isValid())
            return;

        QTextCursor cursor = ui->textEdit->textCursor();
        if (cursor.hasSelection())
        {
            QTextCharFormat fmt = cursor.charFormat();
            fmt.setBackground(color);
            cursor.setCharFormat(fmt);
            ui->textEdit->setTextCursor(cursor);
        }
}


void MainWindow::on_actionToolbar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolbar->setChecked(!visible);
}


void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(!maybeSave())
        {
            return;
        }
        this->close();
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col = 0;
        int ln = 0;
        int flg = -1;
        int pos = ui->textEdit->textCursor().position();
        QString text = ui->textEdit->toPlainText();

        for (int i = 0; i < pos; i++)
        {
            if (text[i] == '\n')
            {
                ln ++;
                flg = i;
            }
        }
        flg ++;
        col = pos - flg;
        statusCursorLabel.setText("Ln: " + QString::number(ln + 1) + "  Col: " + QString::number(col + 1));
}

