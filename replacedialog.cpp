#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>
#include <QTextDocument>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit* textEdit):
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);

     pTextEdit = textEdit;
     ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btfindnext_clicked()
{
    QString target = ui->findtxt->text().trimmed();
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


void ReplaceDialog::on_btreplace_clicked()
{
    QString target = ui->findtxt->text();
    QString to = ui->targetxt->text();
    Qt::CaseSensitivity cs = ui->cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (pTextEdit == nullptr || target.isEmpty())
    {
        QMessageBox::warning(this, tr("替换提示"), tr("查找内容不能为空！"));
        return;
    }

    // 当前选中内容与查找目标一致才替换；替换为空串即删除该处内容
    QString selText = pTextEdit->textCursor().selectedText();
    if (selText.compare(target, cs) == 0)
    {
        pTextEdit->insertPlainText(to);
    }
    // 替换后自动定位到下一处，形成连续的查找—替换过程
    on_btfindnext_clicked();
}


void ReplaceDialog::on_btallreplace_clicked()
{
    QString target = ui->findtxt->text();
    QString to = ui->targetxt->text();

    if (pTextEdit == nullptr || target.isEmpty())
    {
        QMessageBox::warning(this, tr("替换提示"), tr("查找内容不能为空！"));
        return;
    }

    QTextDocument::FindFlags flags;
    if (ui->cbCaseSensitive->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }

    int count = 0;
    QTextCursor cursor = pTextEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    QTextCursor found = pTextEdit->document()->find(target, cursor, flags);

    cursor.beginEditBlock();          // 全部替换合并为一次撤销记录
    while (!found.isNull())
    {
        found.insertText(to);         // 用文档游标替换，保留原有字符格式和撤销栈
        ++count;
        found = pTextEdit->document()->find(target, found, flags);
    }
    cursor.endEditBlock();

    if (count == 0)
    {
        QMessageBox::information(this, tr("替换结果"), tr("未找到“%1”，未进行替换。").arg(target));
    }
    else
    {
        QMessageBox::information(this, tr("替换结果"), tr("共替换 %1 处。").arg(count));
    }
}


void ReplaceDialog::on_btcancel_clicked()
{
    accept();
}

