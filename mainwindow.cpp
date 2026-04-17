#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , resultShown(false)
{
    ui->setupUi(this);

    /*=====================================================================
                            цифры и точка
    =====================================================================*/
    connect(ui->btn0,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn1,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn2,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn3,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn4,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn5,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn6,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn7,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn8,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btn9,   &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    connect(ui->btnDot, &QPushButton::clicked, this, &MainWindow::onDigitClicked);

    /*=====================================================================
                            бинарные операторы
    =====================================================================*/
    connect(ui->btnPlus,  &QPushButton::clicked, this, &MainWindow::onBinaryOperatorClicked);
    connect(ui->btnMinus, &QPushButton::clicked, this, &MainWindow::onBinaryOperatorClicked);
    connect(ui->btnMult,  &QPushButton::clicked, this, &MainWindow::onBinaryOperatorClicked);
    connect(ui->btnDiv,   &QPushButton::clicked, this, &MainWindow::onBinaryOperatorClicked);

    /*=====================================================================
                            скобки
    =====================================================================*/
    connect(ui->btnOpen,  &QPushButton::clicked, this, &MainWindow::onOpenParenClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::onCloseParenClicked);

    /*=====================================================================
                            функции
    =====================================================================*/
    connect(ui->btnSin,   &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->btnCos,   &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->btnTg,    &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->btnCtg,   &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->btnSqrt,  &QPushButton::clicked, this, &MainWindow::onFunctionClicked);
    connect(ui->btnRecip, &QPushButton::clicked, this, &MainWindow::onFunctionClicked);

    /*=====================================================================
                        равно / очистка / удаление
    =====================================================================*/
    connect(ui->btnEqual, &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->btnDel,   &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    /*=====================================================================
                            память (M+, M−, MR, MC)
    =====================================================================*/
    connect(ui->btnMPlus,  &QPushButton::clicked, this, &MainWindow::onMPlusClicked);
    connect(ui->btnMMinus, &QPushButton::clicked, this, &MainWindow::onMMinusClicked);
    connect(ui->btnMR,     &QPushButton::clicked, this, &MainWindow::onMRClicked);
    connect(ui->btnMC,     &QPushButton::clicked, this, &MainWindow::onMCClicked);

    /*=====================================================================
                            undo & redo
    =====================================================================*/
    connect(ui->btnUndo, &QPushButton::clicked, this, &MainWindow::onUndoClicked);
    connect(ui->btnRedo, &QPushButton::clicked, this, &MainWindow::onRedoClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*=====================================================================
                        вспомогательные методы
=====================================================================*/

QString MainWindow::formatResult(double value) const
{
    if (value == std::floor(value) && std::abs(value) < 1e15)
        return QString::number(static_cast<long long>(value));
    return QString::number(value, 'g', 15);
}

void MainWindow::appendToDisplay(const QString& text)
{
    ui->display->setText(ui->display->text() + text);
}

void MainWindow::flushNumberBuffer()
{
    if (numberBuffer.isEmpty()) return;
    facade.addNumber(numberBuffer.toDouble());
    numberBuffer.clear();
}

/*=====================================================================
                        цифры и точка
=====================================================================*/

void MainWindow::onDigitClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    if (resultShown) {
        ui->display->clear();
        facade.clearTokenQueue();
        numberBuffer.clear();
        resultShown = false;
    }

    numberBuffer += btn->text();
    appendToDisplay(btn->text());
}

/*=====================================================================
                        бинарные операторы
=====================================================================*/

void MainWindow::onBinaryOperatorClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    if (resultShown) {
        // продолжаем вычисление от последнего результата
        facade.addNumber(facade.getCurrentResult());
        resultShown = false;
    } else {
        flushNumberBuffer();
    }

    facade.addBinaryOperator(btn->text().at(0).toLatin1());
    appendToDisplay(btn->text());
}

/*=====================================================================
                            скобки
=====================================================================*/

void MainWindow::onOpenParenClicked()
{
    if (resultShown) {
        ui->display->clear();
        facade.clearTokenQueue();
        resultShown = false;
    }

    flushNumberBuffer();
    facade.addOpenParen();
    appendToDisplay("(");
}

void MainWindow::onCloseParenClicked()
{
    flushNumberBuffer();
    facade.addCloseParen();
    appendToDisplay(")");
    resultShown = false;
}

/*=====================================================================
                            функции
=====================================================================*/

void MainWindow::onFunctionClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString funcName = btn->text();
    if (funcName == "1/x") funcName = "recip";

    if (resultShown) {
        const double currentResult = facade.getCurrentResult();

        facade.clearTokenQueue();
        facade.addUnaryFunction(funcName.toStdString());
        facade.addOpenParen();
        facade.addNumber(currentResult);
        facade.addCloseParen();

        ui->display->setText(funcName + "(" + formatResult(currentResult) + ")");
        onEqualClicked();
    } else {
        flushNumberBuffer();
        facade.addUnaryFunction(funcName.toStdString());
        facade.addOpenParen();
        appendToDisplay(funcName + "(");
    }
}

/*=====================================================================
                    равно / очистка / удаление
=====================================================================*/

void MainWindow::onEqualClicked()
{
    flushNumberBuffer();
    const QString expression = ui->display->text().trimmed();
    if (expression.isEmpty()) return;

    try {
        const double result = facade.calculateQueue();
        facade.computeAndSave(expression.toStdString(), result);
        ui->display->setText(formatResult(result));
        resultShown = true;
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", QString::fromStdString(e.what()));
        facade.clearTokenQueue();
    }
}

void MainWindow::onClearClicked()
{
    ui->display->clear();
    facade.clearTokenQueue();
    numberBuffer.clear();
    resultShown = false;
}

void MainWindow::onDeleteClicked()
{
    if (resultShown) {
        onClearClicked();
        return;
    }

    QString text = ui->display->text();
    if (text.isEmpty()) return;

    text.chop(1);
    ui->display->setText(text);

    if (!numberBuffer.isEmpty())
        numberBuffer.chop(1);
    else
        facade.removeLastToken();
}

/*=====================================================================
                        память (M+, M−, MR, MC)
=====================================================================*/

void MainWindow::onMPlusClicked()
{
    bool ok = false;
    const double value = ui->display->text().trimmed().toDouble(&ok);
    if (!ok)
        QMessageBox::warning(this, "Error",
                             "Unable to save value: an expression has been entered on the screen");
    else
        facade.addToMemory(value);
}

void MainWindow::onMMinusClicked()
{
    bool ok = false;
    const double value = ui->display->text().trimmed().toDouble(&ok);
    if (!ok)
        QMessageBox::warning(this, "Error",
                             "Unable to save value: an expression has been entered on the screen");
    else
        facade.subtractFromMemory(value);
}

void MainWindow::onMRClicked()
{
    ui->display->setText(formatResult(facade.memoryRecall()));
    resultShown = true;
}

void MainWindow::onMCClicked()
{
    facade.clearMemory();
}

/*=====================================================================
                            undo & redo
=====================================================================*/

void MainWindow::onUndoClicked()
{
    if (!facade.canUndo()) return;
    facade.undo();
    ui->display->setText(formatResult(facade.getCurrentResult()));
    resultShown = true;
}

void MainWindow::onRedoClicked()
{
    if (!facade.canRedo()) return;
    facade.redo();
    ui->display->setText(formatResult(facade.getCurrentResult()));
    resultShown = true;
}
