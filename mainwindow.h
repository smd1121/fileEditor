#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QTextEdit>
#include <QPrinter>
#include <QPainter>
#include <QCloseEvent>
#include <QShortcut>

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("未命名");

        // 创建文本编辑框
        textEdit = new MyTextEdit(this);
        setCentralWidget(textEdit);

        // 将 textEdit 和 document 关联；此后对一方的更改会在另一方同步
        textEdit->setDocument(document);
        textEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

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
    // new 一个 QAction 并设置其 Icon, Tip 和 Slot
    template<typename Func>
    QAction * initQAction(const QString &filename, const QString &tip, Func slot, std::optional<QKeySequence> shortcut) {
        auto * btn = new QAction(this);

        btn->setIcon(QIcon{filename});
//        qDebug() << QString("已设置按钮 [%1] 的图标，图标 [%2] 空").arg(tip, (btn->icon().isNull() ? "为" : "不为"));

        btn->setToolTip(tip);

        if (shortcut)
            btn->setShortcut(shortcut.value());

        connect(btn, &QAction::triggered, this, slot);
        return btn;
    }

    void initToolbars() {
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
        stylesTb->addAction(initQAction(":/icons/style-size.png", "字号", &MainWindow::styleSize, {}));
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
        utilsTb->addAction(initQAction(":/icons/util-find.png", "查找 (Ctrl+F)", &MainWindow::utilFind, QKeySequence(Qt::CTRL | Qt::Key_F)));
        utilsTb->addAction(initQAction(":/icons/util-replace.png", "替换 (Ctrl+R)", &MainWindow::utilReplace, QKeySequence(Qt::CTRL | Qt::Key_R)));
//        utilsTb->addAction(initQAction(":/icons/util-zoomin.png", "放大", &MainWindow::utilZoomIn, QKeySequence(Qt::CTRL | Qt::Key_Plus)));
//        utilsTb->addAction(initQAction(":/icons/util-zoomout.png", "缩小", &MainWindow::utilZoomOut, QKeySequence(Qt::CTRL | Qt::Key_Minus)));
        addToolBar(utilsTb);

        qDebug() << "初始化工具栏：initToolbars 完成";
    }

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExport();

    void styleBold();
    void styleItalic();
    void styleUnderline();
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
    void editUndo();
    void editRedo();

    void utilFind();
    void utilReplace();
    void utilZoomIn() { zoom(0.1); }
    void utilZoomOut() { zoom(-0.1); }

    void insertText1() {
        QTextCursor cursor(document);
        cursor.movePosition(QTextCursor::End);

        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        cursor.setCharFormat(format);

        cursor.insertText("New Text");
    }

    void insertText2() {
//        auto *textEdit = qobject_cast<QTextEdit*>(centralWidget());


        if (textEdit)
            textEdit->insertPlainText("文本2 ");
    }

private:
    class MyTextEdit : public QTextEdit {
    public:
        explicit MyTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}
        void customPaste() {}
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

    bool checkFileSave();   // 询问是否保存当前文件，如果为是或否，返回 true。返回 false 表示取消。
    void updateFilePath(const QString &path);
    void setAlignment(Qt::AlignmentFlag alignment);
    void zoom(qreal delta);
};