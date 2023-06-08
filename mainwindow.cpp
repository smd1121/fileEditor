#include "mainwindow.h"
#include <QPdfWriter>
#include <QTextDocumentWriter>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>

void MainWindow::fileNew() {
    qDebug() << "新建文件";
    if (!checkFileSave())
        return;

    textEdit->clear();
    filePath.reset();
    setWindowTitle("未命名");
    qDebug() << "新建文件完成";
//    document->addResource(QTextDocument::ImageResource, QUrl(":/icons/file-new.png"),
//                          QImage(":/icons/file-new.png"));
//    QTextCursor cursor(document);
//    cursor.movePosition(QTextCursor::End);
//    QTextImageFormat imageFormat;
//    imageFormat.setName(":/icons/file-new.png");
//    cursor.insertImage(imageFormat);
}

void MainWindow::fileOpen() {
    qDebug() << "打开文件";

    if (!checkFileSave())
        return;

    QString path = QFileDialog::getOpenFileName(this, "打开文件", QString(), "HTML Files (*.html)");

    if (!path.isEmpty()) {
        QFile file(path);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();

            document->setHtml(content);
            document->setModified(false);
            filePath = path;
            updateFilePath(path);

            qDebug() << QString{"文件 %1 打开成功"}.arg(path);
        } else {
            qWarning() << QString{"文件 %1 打开失败"}.arg(path);
        }
    } else {
        qWarning() << "打开 - 用户没有选择路径";
    }
}

void MainWindow::fileSave() {
    qDebug() << "保存文件";

    if (!filePath)
        return fileSaveAs();

    QTextDocumentWriter writer(filePath.value());
    writer.setFormat("HTML");

    bool success = writer.write(document);
    if (success)
        document->setModified(false);

    qDebug() << QString{"保存文件 %1"}.arg(success ? "成功" : "失败");
}

void MainWindow::fileSaveAs() {
    qDebug() << "另存为";
    QString path = QFileDialog::getSaveFileName(this, "存储为", QString(), "HTML Files (*.html)");

    if (!path.isEmpty()) {
        QTextDocumentWriter writer(path);
        writer.setFormat("HTML");  // 设置保存格式为 HTML

        bool success = writer.write(document);  // 将当前文档保存为 HTML
        if (success) {
            document->setModified(false);
            updateFilePath(path);
        }

        qDebug() << QString{"另存为 %1 %2"}.arg(path, success ? "成功" : "失败");
    } else {
        qWarning() << "另存为 - 用户没有选择路径";
    }
}

void MainWindow::fileExport() {
    qDebug() << "导出为 PDF";

    QString fileName = QFileDialog::getSaveFileName(nullptr, "导出 PDF", QString(), "*.pdf");
    if (fileName.isEmpty()) {
        qDebug() << "用户选择路径为空";
        return;
    }

    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }
    qDebug() << "导出到" << fileName;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);  // 设置输出文件名

    document->setPageSize(printer.pageRect(QPrinter::Point).size()); // This is necessary if you want to hide the page number
    document->print(&printer);


//    QPainter painter;
//    painter.begin(&printer);
//    document->drawContents(&painter);
//    painter.end();

//    QPdfWriter writer("out.pdf");
//    writer.setPageSize(QPageSize(QPageSize::A4));
//    writer.setResolution(150);
//
//    QPainter painter(&writer);
//    document->drawContents(&painter);
//    painter.end();
    qDebug() << "导出完成";
}

void MainWindow::styleBold() {
    QTextCursor cursor = textEdit->textCursor(); // 获取文本光标
    QTextCharFormat charFormat = cursor.charFormat(); // 获取当前字符格式

    // 检查当前是否已应用加粗样式
    if (charFormat.fontWeight() == QFont::Bold) {
        charFormat.setFontWeight(QFont::Normal); // 改为不加粗
    } else {
        charFormat.setFontWeight(QFont::Bold); // 改为加粗
    }

    cursor.mergeCharFormat(charFormat); // 合并字符格式
    textEdit->setTextCursor(cursor); // 更新文本光标
}

void MainWindow::styleItalic() {
    QTextCursor cursor = textEdit->textCursor(); // 获取文本光标
    QTextCharFormat charFormat = cursor.charFormat(); // 获取当前字符格式

    charFormat.setFontItalic(!charFormat.fontItalic());

    cursor.mergeCharFormat(charFormat); // 合并字符格式
    textEdit->setTextCursor(cursor); // 更新文本光标
}

void MainWindow::styleUnderline() {
    QTextCursor cursor = textEdit->textCursor(); // 获取文本光标
    QTextCharFormat charFormat = cursor.charFormat(); // 获取当前字符格式

    charFormat.setFontUnderline(!charFormat.fontUnderline());

    cursor.mergeCharFormat(charFormat); // 合并字符格式
    textEdit->setTextCursor(cursor); // 更新文本光标
}

void MainWindow::styleSize() {

}

void MainWindow::styleColor() {

}

void MainWindow::styleFont() {

}

void MainWindow::editPaste() {
    textEdit->paste();
    qDebug() << "粘贴: " << QGuiApplication::clipboard()->text();
}

void MainWindow::editCopy() {
    textEdit->copy();
    qDebug() << "拷贝: " << QGuiApplication::clipboard()->text();
}

void MainWindow::editCut() {
    textEdit->cut();
    qDebug() << "剪切: " << QGuiApplication::clipboard()->text();
}

void MainWindow::editImage() {

}

void MainWindow::editUndo() {

}

void MainWindow::editRedo() {

}

void MainWindow::utilFind() {

}

void MainWindow::utilReplace() {

}

bool MainWindow::checkFileSave() {
    qDebug() << "检查是否更改：" << (document->isModified() ? "是" : "否");
    if (document->isModified()) {
        auto answer = QMessageBox::question(this, "是否保存当前更改？",
                                            "是否保存当前更改？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (answer == QMessageBox::Yes) {
            qDebug() << "用户选择保存";
            fileSave();
            return true;
        } else if (answer == QMessageBox::No) {
            qDebug() << "用户选择不保存";
            return true;
        } else {  // answer == QMessageBox::Cancel
            qDebug() << "用户选择取消";
            return false;
        }
    }
    return true;
}

void MainWindow::updateFilePath(const QString &path) {
    filePath = path;
    setWindowTitle(path);
}

void MainWindow::setAlignment(Qt::AlignmentFlag alignment) {
    QTextCursor cursor = textEdit->textCursor(); // 获取文本编辑框的光标
    QTextBlockFormat blockFormat = cursor.blockFormat(); // 获取当前段落的格式

    blockFormat.setAlignment(alignment); // 设置对齐方式

    cursor.setBlockFormat(blockFormat); // 应用段落格式
    textEdit->setTextCursor(cursor); // 更新文本编辑框的光标
}

void MainWindow::zoom(qreal delta) {
    static qreal zoomFactor = 1.0;
    zoomFactor += delta;
    QString css = QString("body { transform: scale(%1); }").arg(zoomFactor);
    textEdit->document()->setDefaultStyleSheet(css);
}
