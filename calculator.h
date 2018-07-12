#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QMainWindow>

namespace Ui {
class calculator;
}

class calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit calculator(QWidget *parent = 0);
    ~calculator();

private slots:
    void digitClicked();
    void equalClicked();
    void lateradderOperatorClicked();
    void latermulOperatorClicked();
    void pointClicked();
    void change();
    void DELClicked();
    void clearALL();
    void unaryfunction();
    void Adsclicked();
    //    void sinOperatorClicked();
    //    void arcsinOperatorClicked();
    //    void cosOperatorClicked();
    //    void arccosOperatorClicked();
    //    void tanOperatorClicked();
    //    void arctanOperatorClicked();
    //    void leftOperatorClicked();
    //    void rightOperatorClicked();
    //    void sqrtClicked();
    //    void squareClicked();
    //    void adderOperatorClicked();
    //    void multipliedOperatorClicked();
    //    void subtractionOperatorClicked();
    //   void divisionOperatorClicked();

private:
    Ui::calculator *ui;

    double getPi();
    void abortOperation();
    bool Calculate(double rightOperand, const QString &pendingOperator);
    
    double SumSoFar;
    double FactorSoFar;
    const double pi = 3.14159265358979;

    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    
    bool waitingForOperand;

};

#endif // CALCULATOR_H

