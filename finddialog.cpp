#include "finddialog.h"
#include "ui_finddialog.h"
#include <QMessageBox>


FindDialog::FindDialog(QWidget *parent , QPlainTextEdit * textEdit) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;
    ui->rbDown->setChecked(true);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_btFindNext_clicked()
{
    QString target = ui->Findtxt->text().trimmed();
        if(target.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("查找提示"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("查找内容不能为空！"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        QString text = pTextEdit->toPlainText();
        QTextCursor c = pTextEdit->textCursor();
        int index = -1;
        Qt::CaseSensitivity cs = ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

        if (ui->rbDown->isChecked())
        {
            //向下查找
            index = text.indexOf(target, c.position(), cs);
            if (index >= 0)
            {
                c.setPosition(index);
                c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
                pTextEdit->setTextCursor(c);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("查找结果"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("已到达文档末尾，未找到匹配内容。"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
        else if (ui->rbUp->isChecked())
        {
            //向上查找
            index = text.lastIndexOf(target, c.position() - 1, cs);
            if (index >= 0)
            {
                c.setPosition(index + target.length());
                c.setPosition(index, QTextCursor::KeepAnchor);
                pTextEdit->setTextCursor(c);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("查找结果"));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("已到达文档开头，未找到匹配内容。"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
            }
        }
}


void FindDialog::on_btFindCancel_clicked()
{
    accept();
}

