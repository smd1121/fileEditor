#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QTextEdit>
#include <QPrinter>
#include <QPainter>
#include <QCloseEvent>
#include <QShortcut>
#include <QLineEdit>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include "FindReplaceDialog.h"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("未命名");

        // 创建文本编辑框
        textEdit = new MyTextEdit(this);

        // 将 textEdit 和 document 关联；此后对一方的更改会在另一方同步
        textEdit->setDocument(document);
        textEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

        QFont defaultFont;
        defaultFont.setPointSize(14);
        document->setDefaultFont(defaultFont);

        auto cursor = textEdit->textCursor();
        auto format = cursor.charFormat();
        qDebug() << "光标默认字号：" << format.fontPointSize();
        if (format.fontPointSize() == 0) {
            format.setFontPointSize(14);
            cursor.setCharFormat(format);
            textEdit->setTextCursor(cursor);
            qDebug() << "光标字号：" << textEdit->textCursor().charFormat().fontPointSize();
        }

        setCentralWidget(textEdit);

        // 初始化工具栏
        initToolbars();
    }

    void closeEvent(QCloseEvent* event) override {
        if (checkFileSave()) {
            event->accept();  // 允许关闭窗口
        } else {
            event->ignore();  // 忽略关闭事件
        }
    }

private:
    void initToolbars();

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExport();

    void styleBold();
    void styleItalic();
    void styleUnderline();
    void showStyleSize();
    void styleSize();
    void styleColor();
    void styleFont();

    void alignLeft() { setAlignment(Qt::AlignLeft); }
    void alignCenter() { setAlignment(Qt::AlignCenter); }
    void alignRight() { setAlignment(Qt::AlignRight); }

    void editPaste();
    void editCopy();
    void editCut();
    void editImage();
    void editUndo() { textEdit->undo(); }
    void editRedo() { textEdit->redo(); }

    void utilFindAndReplace() { findReplaceDialog->show(); }
    void handleFindText(const QString &text, bool alertOnNotFound = true);
    void handleReplaceText(const QString &searchText, const QString &replaceText);
    void handleReplaceAllText(const QString &searchText, const QString &replaceText);
//    void utilZoomIn() { zoom(0.1); }
//    void utilZoomOut() { zoom(-0.1); }

private:
    // new 一个 QAction 并设置其 Icon, Tip 和 Slot
    template<typename Func>
    QAction * initQAction(const QString &filename, const QString &tip, Func slot, std::optional<QKeySequence> shortcut, bool enable = true) {
        auto * btn = new QAction(this);

        btn->setIcon(QIcon{filename});

        btn->setToolTip(tip);

        if (shortcut)
            btn->setShortcut(shortcut.value());

        btn->setEnabled(enable);

        connect(btn, &QAction::triggered, this, slot);
        return btn;
    }

    class MyTextEdit : public QTextEdit {
    public:
        explicit MyTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}
        void customPaste();
    protected:
        void keyPressEvent(QKeyEvent *event) override {
            if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V) {
                // 处理 Ctrl + V 快捷键
                customPaste();
                return;
            }

            // 其他情况下，继续处理默认的键盘事件
            QTextEdit::keyPressEvent(event);
        }
    };

    QTextDocument *document = new QTextDocument();
    MyTextEdit *textEdit;
    std::optional<QString> filePath{};
    QLineEdit* fontSizeInput{new QLineEdit(this)};

    bool checkFileSave();   // 询问是否保存当前文件，如果为是或否，返回 true。返回 false 表示取消。
    void updateFilePath(const QString &path);
    void setAlignment(Qt::AlignmentFlag alignment);
    void zoom(qreal delta);

    static constexpr int maxImgWidth = 575;
    static void addImage(MyTextEdit *textEdit, QImage &image);

    FindReplaceDialog *findReplaceDialog{new FindReplaceDialog(this)};
};

#endif //MAINWINDOW_H