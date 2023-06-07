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
        // 创建工具栏
        auto *toolbar = new QToolBar(this);
        addToolBar(toolbar);

        // 创建工具按钮
        auto *toolButton1 = new QAction("按钮1", this);
        auto *toolButton2 = new QAction("按钮2", this);
        toolbar->addAction(toolButton1);
        toolbar->addAction(toolButton2);

        // 创建文本编辑框
        textEdit = new QTextEdit(this);
        setCentralWidget(textEdit);

        // 将 textEdit 和 document 关联；此后对一方的更改会在另一方同步
        textEdit->setDocument(document);

        // 连接工具按钮的信号与槽函数
//        connect(toolButton1, &QAction::triggered, this, &MainWindow::insertText1);
//        connect(toolButton2, &QAction::triggered, this, &MainWindow::insertText2);

        initToolbars();
    }

private:
    // new 一个 QAction 并设置其 Icon, Tip 和 Slot
    template<typename Func>
    QAction * initQAction(const QString &filename, const QString &tip, Func slot) {
        auto * btn = new QAction(this);

        btn->setIcon(QIcon{filename});
        qDebug() << QString("已设置按钮 [%1] 的图标，图标 [%2] 空").arg(tip, (btn->icon().isNull() ? "为" : "不为"));

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
        qDebug() << "初始化工具栏：initToolbars 完成";
    }

private slots:
    void fileNew() { qDebug() << "fileNew"; }
    void fileOpen() { qDebug() << "fileNew"; }
    void fileSave() { qDebug() << "fileNew"; }
    void fileSaveAs() { qDebug() << "fileNew"; }
    void fileExport() { qDebug() << "fileNew"; }

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