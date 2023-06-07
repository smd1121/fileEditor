#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QTextEdit>
#include <QPrinter>
#include <QPainter>

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // 创建文本编辑框
        textEdit = new QTextEdit(this);
        setCentralWidget(textEdit);

        // 将 textEdit 和 document 关联；此后对一方的更改会在另一方同步
        textEdit->setDocument(document);

        // 初始化工具栏
        initToolbars();
    }

private:
    // new 一个 QAction 并设置其 Icon, Tip 和 Slot
    template<typename Func>
    QAction * initQAction(const QString &filename, const QString &tip, Func slot) {
        auto * btn = new QAction(this);

        btn->setIcon(QIcon{filename});
//        qDebug() << QString("已设置按钮 [%1] 的图标，图标 [%2] 空").arg(tip, (btn->icon().isNull() ? "为" : "不为"));

        btn->setToolTip(tip);
        connect(btn, &QAction::triggered, this, slot);
        return btn;
    }

    void initToolbars() {
        qDebug() << "初始化工具栏：initToolbars";
        auto * filesTb = new QToolBar(this);
        filesTb->addAction(initQAction(":/icons/file-new.png", "新建文件 (Ctrl+N)", &MainWindow::fileNew));
        filesTb->addAction(initQAction(":/icons/file-open.png", "打开文件 (Ctrl+O)", &MainWindow::fileOpen));
        filesTb->addAction(initQAction(":/icons/file-save.png", "保存文件 (Ctrl+S)", &MainWindow::fileSave));
        filesTb->addAction(initQAction(":/icons/file-saveas.png", "另存为", &MainWindow::fileSaveAs));
        filesTb->addAction(initQAction(":/icons/file-export.png", "导出为 PDF", &MainWindow::fileExport));
        addToolBar(filesTb);

        auto * stylesTb = new QToolBar(this);
        stylesTb->addAction(initQAction(":/icons/style-bold.png", "加粗 (Ctrl+B)", &MainWindow::styleBold));
        stylesTb->addAction(initQAction(":/icons/style-italic.png", "斜体 (Ctrl+I)", &MainWindow::styleItalic));
        stylesTb->addAction(initQAction(":/icons/style-underline.png", "下划线 (Ctrl+U)", &MainWindow::styleUnderline));
        stylesTb->addAction(initQAction(":/icons/style-size.png", "字号", &MainWindow::styleSize));
        stylesTb->addAction(initQAction(":/icons/style-color.png", "颜色", &MainWindow::styleColor));
        stylesTb->addAction(initQAction(":/icons/style-font.png", "字体", &MainWindow::styleFont));
        addToolBar(stylesTb);

        auto * alignsTb = new QToolBar(this);
        alignsTb->addAction(initQAction(":/icons/align-left.png", "左对齐 (Ctrl+L)", &MainWindow::alignLeft));
        alignsTb->addAction(initQAction(":/icons/align-center.png", "居中 (Ctrl+E)", &MainWindow::alignCenter));
        alignsTb->addAction(initQAction(":/icons/align-right.png", "右对齐 (Ctrl+R)", &MainWindow::alignRight));
        addToolBar(alignsTb);

        auto * editTb = new QToolBar(this);
        editTb->addAction(initQAction(":/icons/edit-paste.png", "粘贴 (Ctrl+V)", &MainWindow::editPaste));
        editTb->addAction(initQAction(":/icons/edit-copy.png", "拷贝 (Ctrl+C)", &MainWindow::editCopy));
        editTb->addAction(initQAction(":/icons/edit-cut.png", "剪切 (Ctrl+X)", &MainWindow::editCut));
        editTb->addAction(initQAction(":/icons/edit-image.png", "插入图片", &MainWindow::editImage));
        editTb->addAction(initQAction(":/icons/edit-undo.png", "撤销 (Ctrl+Z)", &MainWindow::editUndo));
        editTb->addAction(initQAction(":/icons/edit-redo.png", "重做 (Ctrl+Y)", &MainWindow::editRedo));
        addToolBar(editTb);

        auto * utilsTb = new QToolBar(this);
        utilsTb->addAction(initQAction(":/icons/util-find.png", "查找 (Ctrl+F)", &MainWindow::utilFind));
        utilsTb->addAction(initQAction(":/icons/util-replace.png", "替换 (Ctrl+R)", &MainWindow::utilReplace));
        utilsTb->addAction(initQAction(":/icons/util-zoomin.png", "放大", &MainWindow::utilZoomIn));
        utilsTb->addAction(initQAction(":/icons/util-zoomout.png", "缩小", &MainWindow::utilZoomOut));
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

    void alignLeft();
    void alignCenter();
    void alignRight();

    void editPaste();
    void editCopy();
    void editCut();
    void editImage();
    void editUndo();
    void editRedo();

    void utilFind();
    void utilReplace();
    void utilZoomIn();
    void utilZoomOut();

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
        document->addResource(QTextDocument::ImageResource, QUrl(":/icons/file-new.png"),
                              QImage(":/icons/file-new.png"));
        QTextCursor cursor(document);
        cursor.movePosition(QTextCursor::End);
        QTextImageFormat imageFormat;
        imageFormat.setName(":/icons/file-new.png");
        cursor.insertImage(imageFormat);

        if (textEdit)
            textEdit->insertPlainText("文本2 ");
    }

private:
    QTextDocument *document = new QTextDocument();
    QTextEdit *textEdit;
};