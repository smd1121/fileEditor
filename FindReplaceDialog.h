#ifndef FILEEDITOR_FINDREPLACEDIALOG_H
#define FILEEDITOR_FINDREPLACEDIALOG_H

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

class FindReplaceDialog : public QDialog {
    Q_OBJECT

public:
    explicit FindReplaceDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setupUI();
    }

    signals:
    void findText(const QString &text, bool);
    void replaceText(const QString &searchText, const QString &replaceText);
    void replaceAllText(const QString &searchText, const QString &replaceText);

private slots:
    void onFindButtonClicked() {
        QString searchText = findLineEdit->text();
        emit findText(searchText, true);
    }

    void onReplaceButtonClicked() {
        QString searchText = findLineEdit->text();
        QString newText = replaceLineEdit->text();
        emit replaceText(searchText, newText);
    }

    void onReplaceAllButtonClicked() {
        QString searchText = findLineEdit->text();
        QString newText = replaceLineEdit->text();
        emit replaceAllText(searchText, newText);
    }

private:
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;

    void setupUI() {
        findLineEdit = new QLineEdit(this);
        findLineEdit->setPlaceholderText("搜索文本");
        replaceLineEdit = new QLineEdit(this);
        replaceLineEdit->setPlaceholderText("替换为");

        findButton = new QPushButton("查找下一处", this);
        replaceButton = new QPushButton("替换", this);
        replaceAllButton = new QPushButton("全部替换", this);

        auto *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(findButton);
        buttonLayout->addWidget(replaceButton);
        buttonLayout->addWidget(replaceAllButton);

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(findLineEdit);
        mainLayout->addWidget(replaceLineEdit);
        mainLayout->addLayout(buttonLayout);

        connect(findButton, &QPushButton::clicked, this, &FindReplaceDialog::onFindButtonClicked);
        connect(replaceButton, &QPushButton::clicked, this, &FindReplaceDialog::onReplaceButtonClicked);
        connect(replaceAllButton, &QPushButton::clicked, this, &FindReplaceDialog::onReplaceAllButtonClicked);

        setWindowTitle("查找和替换");
    }
};


#endif //FILEEDITOR_FINDREPLACEDIALOG_H
