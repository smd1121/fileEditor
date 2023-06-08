#include "mainwindow.h"
#include <QPdfWriter>
#include <QTextDocumentWriter>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QVBoxLayout>
#include <QMimeData>
#include <QImageWriter>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>

void MainWindow::initToolbars() {
    qDebug() << "初始化工具栏：initToolbars";
    auto * filesTb = new QToolBar(this);
    filesTb->addAction(initQAction(":/icons/file-new.png", "新建文件 (Ctrl+N)", &MainWindow::fileNew, QKeySequence(Qt::CTRL | Qt::Key_N)));
    filesTb->addAction(initQAction(":/icons/file-open.png", "打开文件 (Ctrl+O)", &MainWindow::fileOpen, QKeySequence(Qt::CTRL | Qt::Key_O)));
    filesTb->addAction(initQAction(":/icons/file-save.png", "保存文件 (Ctrl+S)", &MainWindow::fileSave, QKeySequence(Qt::CTRL | Qt::Key_S)));
    filesTb->addAction(initQAction(":/icons/file-saveas.png", "另存为", &MainWindow::fileSaveAs, {}));
    filesTb->addAction(initQAction(":/icons/file-export.png", "导出为 PDF", &MainWindow::fileExport, {}));
    addToolBar(filesTb);

    auto * stylesTb = new QToolBar(this);
    stylesTb->addAction(initQAction(":/icons/style-bold.png", "加粗 (Ctrl+B)", &MainWindow::styleBold, QKeySequence(Qt::CTRL | Qt::Key_B)));
    stylesTb->addAction(initQAction(":/icons/style-italic.png", "斜体 (Ctrl+I)", &MainWindow::styleItalic, QKeySequence(Qt::CTRL | Qt::Key_I)));
    stylesTb->addAction(initQAction(":/icons/style-underline.png", "下划线 (Ctrl+U)", &MainWindow::styleUnderline, QKeySequence(Qt::CTRL | Qt::Key_U)));
    stylesTb->addAction(initQAction(":/icons/style-size.png", "字号", &MainWindow::showStyleSize, {}));
    fontSizeInput->setFixedWidth(30);
    stylesTb->addWidget(fontSizeInput);
    // 监听输入框的回车键事件，以确认用户输入
    connect(fontSizeInput, &QLineEdit::returnPressed, this, &MainWindow::styleSize);
    stylesTb->addAction(initQAction(":/icons/style-color.png", "颜色", &MainWindow::styleColor, {}));
    stylesTb->addAction(initQAction(":/icons/style-font.png", "字体", &MainWindow::styleFont, {}));
    addToolBar(stylesTb);

    auto * alignsTb = new QToolBar(this);
    alignsTb->addAction(initQAction(":/icons/align-left.png", "左对齐 (Ctrl+L)", &MainWindow::alignLeft, QKeySequence(Qt::CTRL | Qt::Key_L)));
    alignsTb->addAction(initQAction(":/icons/align-center.png", "居中 (Ctrl+E)", &MainWindow::alignCenter, QKeySequence(Qt::CTRL | Qt::Key_E)));
    alignsTb->addAction(initQAction(":/icons/align-right.png", "右对齐 (Ctrl+R)", &MainWindow::alignRight, QKeySequence(Qt::CTRL | Qt::Key_R)));
    addToolBar(alignsTb);

    auto * editTb = new QToolBar(this);
    editTb->addAction(initQAction(":/icons/edit-paste.png", "粘贴 (Ctrl+V)", &MainWindow::editPaste, QKeySequence(Qt::CTRL | Qt::Key_V)));
    editTb->addAction(initQAction(":/icons/edit-copy.png", "拷贝 (Ctrl+C)", &MainWindow::editCopy, QKeySequence(Qt::CTRL | Qt::Key_C)));
    editTb->addAction(initQAction(":/icons/edit-cut.png", "剪切 (Ctrl+X)", &MainWindow::editCut, QKeySequence(Qt::CTRL | Qt::Key_X)));
    editTb->addAction(initQAction(":/icons/edit-image.png", "插入图片", &MainWindow::editImage, {}));
    editTb->addAction(initQAction(":/icons/edit-undo.png", "撤销 (Ctrl+Z)", &MainWindow::editUndo, QKeySequence(Qt::CTRL | Qt::Key_Z)));
    editTb->addAction(initQAction(":/icons/edit-redo.png", "重做 (Ctrl+Y)", &MainWindow::editRedo, QKeySequence(Qt::CTRL | Qt::Key_Y)));
    addToolBar(editTb);

    auto * utilsTb = new QToolBar(this);
    utilsTb->addAction(initQAction(":/icons/util-find.png", "查找 (Ctrl+F)", &MainWindow::utilFindAndReplace, QKeySequence(Qt::CTRL | Qt::Key_F)));
    utilsTb->addAction(initQAction(":/icons/util-replace.png", "替换 (Ctrl+R)", &MainWindow::utilFindAndReplace, QKeySequence(Qt::CTRL | Qt::Key_R)));
//        utilsTb->addAction(initQAction(":/icons/util-zoomin.png", "放大", &MainWindow::utilZoomIn, QKeySequence(Qt::CTRL | Qt::Key_Plus)));
//        utilsTb->addAction(initQAction(":/icons/util-zoomout.png", "缩小", &MainWindow::utilZoomOut, QKeySequence(Qt::CTRL | Qt::Key_Minus)));
    addToolBar(utilsTb);
    connect(findReplaceDialog, &FindReplaceDialog::findText, this, &MainWindow::handleFindText);
    connect(findReplaceDialog, &FindReplaceDialog::replaceText, this, &MainWindow::handleReplaceText);
    connect(findReplaceDialog, &FindReplaceDialog::replaceAllText, this, &MainWindow::handleReplaceAllText);

    qDebug() << "初始化工具栏：initToolbars 完成";
}

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

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);  // 设置输出文件名
    qDebug() << "导出尺寸: " << printer.pageRect(QPrinter::Point).size().width() << ", " << printer.pageRect(QPrinter::Point).size().height();
//    printer.setPageSize(QPageSize(document->pageSize(), QPageSize::Point));

    auto oldPageSize = document->pageSize();
//    document->setPageSize(printer.pageRect(QPrinter::Point).size()); // This is necessary if you want to hide the page number
    document->print(&printer);
    document->setPageSize(oldPageSize);

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

void MainWindow::showStyleSize() {
    // 获取当前光标所在位置的字号
    QTextCursor cursor = textEdit->textCursor();
    qreal currentFontSize = cursor.charFormat().fontPointSize();

    qDebug() << "显示当前字号：" << currentFontSize;

    // 在输入框中显示当前字号
    fontSizeInput->setText(QString::number(currentFontSize));
}

void MainWindow::styleSize() {
    // 获取当前光标所在位置的字号
    QTextCursor cursor = textEdit->textCursor();

    qDebug() << "设置字号";
    fontSizeInput->clearFocus(); // 移除输入框的焦点，确认用户输入

    bool ok = false;
    int newFontSize = fontSizeInput->text().toInt(&ok);
    if (!ok || newFontSize <= 0) {
        qDebug() << "字号输入非法，放弃";
        return;
    }
    qDebug() << "设置字号为" << newFontSize;

    QTextCharFormat format = cursor.charFormat();
    format.setFontPointSize(newFontSize);
    cursor.setCharFormat(format);
    textEdit->setTextCursor(cursor);
}

void MainWindow::styleColor() {
    QColorDialog dialog(this);
    dialog.setOption(QColorDialog::DontUseNativeDialog); // 禁用系统原生对话框（可选）

    if (dialog.exec() == QDialog::Accepted) {
        QColor selectedColor = dialog.selectedColor();

        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        format.setForeground(selectedColor);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
}

void MainWindow::styleFont() {
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont(), this);
    if (ok) {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        format.setFont(font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
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
    QString imgPath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));
    qDebug() << "打开图片: " << imgPath;
    if (!imgPath.isEmpty()) {
        QImage image(imgPath);
        if (!image.isNull()) {
            qDebug() << "插入图片";
//            document->addResource(QTextDocument::ImageResource, QUrl(imgPath), image);
//            textEdit->textCursor().insertImage(imgPath);
            MainWindow::addImage(textEdit, image);
            return;
        }
    }
    qDebug() << "图片打开失败";
}

//void MainWindow::utilFind() {
//    QString searchText = QInputDialog::getText(this, "查找", "请输入要查找的文本：");
//
//    if (!searchText.isEmpty()) {
//        QTextCursor cursor = textEdit->document()->find(searchText);
//        if (!cursor.isNull()) {
//            textEdit->setTextCursor(cursor);
////            textEdit->centerCursor();
//        } else {
//            QMessageBox::information(this, "查找", "未找到匹配的文本。");
//        }
//    }
//}
//
//void MainWindow::utilReplace() {
//    QString searchText = QInputDialog::getText(this, "查找", "请输入要查找的文本：");
//    QString replaceText = QInputDialog::getText(this, "替换", "请输入要替换的文本：");
//
//    if (!searchText.isEmpty() && !replaceText.isEmpty()) {
//        QTextCursor cursor = textEdit->textCursor();
//        cursor.beginEditBlock();
//
//        if (cursor.hasSelection()) {
//            QString selectedText = cursor.selectedText();
//            if (selectedText == searchText) {
//                cursor.insertText(replaceText);
//            }
//        } else {
//            while (!cursor.isNull() && !cursor.atEnd()) {
//                cursor = textEdit->document()->find(searchText, cursor);
//                if (!cursor.isNull()) {
//                    cursor.insertText(replaceText);
//                }
//            }
//        }
//
//        cursor.endEditBlock();
//    }
//}

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

void MainWindow::MyTextEdit::customPaste() {
    qDebug() << "粘贴";

    auto* clipboard = QGuiApplication::clipboard();
    auto* mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        qDebug() << "发现图片，粘贴";

        auto image = qvariant_cast<QImage>(mimeData->imageData());
        if (!image.isNull()) {
            MainWindow::addImage(this, image);
//            QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
//            QString imgPath = timestamp + ".png";  // 设置图片的保存路径和文件名
//            qDebug() << "图片尺寸: " << image.height();
//            qDebug() << "将图片保存到: " << imgPath;
//            QImageWriter iWriter(imgPath);
//            auto result = iWriter.write(image);
//            if (!result)
//                qDebug() << "保存失败: " << iWriter.errorString();
//            document()->addResource(QTextDocument::ImageResource, QUrl::fromLocalFile(imgPath), image);
//            textCursor().insertImage(imgPath);
            return;
        }
    }

    qDebug() << "粘贴: " << QGuiApplication::clipboard()->text();
    paste();
}

void MainWindow::addImage(MyTextEdit *textEdit, QImage &image) {
    qDebug() << QString{"图片原尺寸: [%1 * %2]"}.arg(image.width()).arg(image.height());

    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    QString imgPath = timestamp + ".png";  // 设置图片的保存路径和文件名
    qDebug() << "将图片保存到: " << imgPath;

    if (image.width() > maxImgWidth) {
        // 缩放
        qreal scaleFactor = static_cast<qreal>(maxImgWidth) / image.width();
        QSize newSize = image.size() * scaleFactor;
        image = image.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << QString{"缩放后尺寸: [%1 * %2]"}.arg(image.width()).arg(image.height());
    }

    QImageWriter iWriter(imgPath);
    auto result = iWriter.write(image);
    if (!result)
        qDebug() << "保存失败: " << iWriter.errorString();
    textEdit->document()->addResource(QTextDocument::ImageResource, QUrl::fromLocalFile(imgPath), image);
    textEdit->textCursor().insertImage(imgPath);

    qDebug() << "插入完成";
}

void MainWindow::handleFindText(const QString &searchText, bool alertOnNotFound) {
    qDebug() << "搜索: " << searchText;
    if (!searchText.isEmpty()) {
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::NextCharacter);
        cursor = textEdit->document()->find(searchText, cursor);
        if (cursor.isNull())
            cursor = textEdit->document()->find(searchText);

        if (!cursor.isNull()) {
            textEdit->setTextCursor(cursor);
        } else if (alertOnNotFound) {
            QMessageBox::information(this, "查找", "未找到匹配的文本。");
        }
    }
}

void MainWindow::handleReplaceText(const QString &searchText, const QString &replaceText) {
    qDebug() << "替换 " << searchText << " 为 " << replaceText;

    if (searchText.isEmpty())   return;

    QTextCursor cursor = textEdit->textCursor();
    qDebug() << "当前选中: " << cursor.selectedText();
    if (cursor.selectedText() == searchText) {
        cursor.insertText(replaceText);
        handleFindText(searchText, false);
    } else {
        handleFindText(searchText);
    }
}

void MainWindow::handleReplaceAllText(const QString &searchText, const QString &replaceText) {
    if (searchText.isEmpty()) return;

    for (QTextCursor cursor = document->find(searchText); !cursor.isNull(); cursor = document->find(searchText)) {
        cursor.insertText(replaceText);
    }
}

