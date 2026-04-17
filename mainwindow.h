#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "calculator_facade.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CalculatorFacade facade;
    bool resultShown;
    QString numberBuffer;

    QString formatResult(double value) const;
    void    appendToDisplay(const QString& text);
    void    flushNumberBuffer();

    void onDigitClicked();
    void onBinaryOperatorClicked();
    void onOpenParenClicked();
    void onCloseParenClicked();
    void onFunctionClicked();
    void onEqualClicked();
    void onClearClicked();
    void onDeleteClicked();
    void onMPlusClicked();
    void onMMinusClicked();
    void onMRClicked();
    void onMCClicked();
    void onUndoClicked();
    void onRedoClicked();
};

#endif // MAINWINDOW_H
