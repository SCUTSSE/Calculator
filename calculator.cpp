#include "calculator.h"
#include "ui_calculator.h"

#include <qmath.h>
calculator::calculator(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::calculator)
{
        setFixedSize(600, 450);
        setWindowIcon(QIcon("calculator.ico"));
        ui->setupUi(this);

        SumSoFar = 0.0;
        FactorSoFar = 0.0;
        waitingForOperand = 1;  //1 means true


        //connect(ui->pushButton_,SIGNAL(clicked()),this,SLOT());

        //numberkey
        connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digitClicked()));
        connect(ui->pushButton_Pi, SIGNAL(clicked()), this, SLOT(digitClicked()));

        //Four operations key
        connect(ui->pushButton_Addition, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));
        connect(ui->pushButton_Subtraction, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));
        connect(ui->pushButton_Multiplication, SIGNAL(clicked()), this, SLOT(latermulOperatorClicked()));
        connect(ui->pushButton_Division, SIGNAL(clicked()), this, SLOT(latermulOperatorClicked()));

        //
        connect(ui->pushButton_point, SIGNAL(clicked()), this, SLOT(pointClicked()));
        connect(ui->pushButton_negative, SIGNAL(clicked()), this, SLOT(change()));

        //clearkey
        connect(ui->pushButton_DEL, SIGNAL(clicked()), this, SLOT(DELClicked()));
        connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(clearALL()));

        //functionkey
        connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_cot, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_arcsin, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_arccos, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_arctan, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_arccot, SIGNAL(clicked()), this, SLOT(unaryfunction()));

        connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_square, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_reciprocal, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_e, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_factorial, SIGNAL(clicked()), this, SLOT(unaryfunction()));
        connect(ui->pushButton_Abs, SIGNAL(clicked()), this, SLOT(Adsclicked()));


        //
        connect(ui->pushButton_MR,SIGNAL(clicked()),this,SLOT(MRclicked()));
        connect(ui->pushButton_MC,SIGNAL(clicked()),this,SLOT(MCclicked()));
        connect(ui->pushButton_Madd,SIGNAL(clicked()),this,SLOT(Maddclicked()));
        connect(ui->pushButton_Msub,SIGNAL(clicked()),this,SLOT(Msubclicked()));

        //
        connect(ui->pushButton_Tensquare, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));
        connect(ui->pushButton_x_y, SIGNAL(clicked()), this, SLOT(lateradderOperatorClicked()));

        //
        connect(ui->pushButton_Equal, SIGNAL(clicked()), this, SLOT(equalClicked()));

}


calculator::~calculator()
{
        delete ui;
}

void calculator::digitClicked()
{
        QPushButton *clicked;
        clicked = qobject_cast<QPushButton *>(sender());

        int value = 0;
        double Pi;
        //    double a = 0;        //temporary variable ,store decimal
        if (clicked->text() == "Pi")
        {
                Pi = getPi();
                ui->lineEdit_output->clear();
        }
        else
        {
                value = clicked->text().toInt();
        }

        if (ui->lineEdit_output->text() == "0" && value == 0)
                return;

        if (waitingForOperand)
        {
                ui->lineEdit_output->clear();
                waitingForOperand = 0;
        }

        if (clicked->text() == "Pi")
        {
                ui->lineEdit_output->setText(QString::number(Pi, 'f', 10));
                waitingForOperand = 1;
        }
        else
        {
                //a=ui->lineEdit_output->text().toDouble() + value;
                ui->lineEdit_output->setText(ui->lineEdit_output->text() + QString::number(value));
        }


}

void calculator::equalClicked()
{
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                a = FactorSoFar;
                FactorSoFar = 0;
                pendingMultiplicativeOperator.clear();
        }
        if (!pendingAdditiveOperator.isEmpty())
        {
                if (!Calculate(a, pendingAdditiveOperator))
                {
                        abortOperation();
                        return;
                }
                pendingAdditiveOperator.clear();
        }
        else
        {
                SumSoFar = a;
        }
        if (SumSoFar >= (10 ^ 6))
        {
                ui->lineEdit_output->setText(QString::number(SumSoFar));
        }
        else
        {
                ui->lineEdit_output->setText(QString::number(SumSoFar, 'f', 10));
        }
        SumSoFar = 0;
        waitingForOperand = 1;
}
void calculator::lateradderOperatorClicked()
{
        QPushButton *clicked = qobject_cast<QPushButton *>(sender());
        QString operatorname = clicked->text();
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                if (FactorSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar, 'f', 10));
                }
                a = FactorSoFar;
                FactorSoFar = 0;
                pendingMultiplicativeOperator.clear();
        }

        if (!pendingAdditiveOperator.isEmpty())
        {
                if (!Calculate(a, pendingAdditiveOperator))
                {
                        abortOperation();
                        return;
                }
                if (SumSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(SumSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(SumSoFar, 'f', 10));
                }
        }
        else
        {
                SumSoFar = a;
        }

        pendingAdditiveOperator = operatorname;

        waitingForOperand = 1;
}

void calculator::latermulOperatorClicked()
{
        QPushButton *clicked = qobject_cast<QPushButton *>(sender());
        QString operatorname = clicked->text();
        double a = ui->lineEdit_output->text().toDouble();

        if (!pendingMultiplicativeOperator.isEmpty())
        {
                if (!Calculate(a, pendingMultiplicativeOperator))
                {
                        abortOperation();
                        return;
                }
                if (FactorSoFar >= (10 ^ 6))
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar));
                }
                else
                {
                        ui->lineEdit_output->setText(QString::number(FactorSoFar, 'f', 10));
                }
        }
        else
        {
                FactorSoFar = a;
        }

        pendingMultiplicativeOperator = operatorname;
        waitingForOperand = 1;
}

void calculator::MCclicked()
{
    sumInMemory=0.0;
}
void calculator::MRclicked()
{
    ui->lineEdit_output->setText(QString::number(sumInMemory));
    waitingForOperand=true;
}
void calculator::Maddclicked()
{
    equalClicked();
    sumInMemory+=ui->lineEdit_output->text().toDouble();
}
void calculator::Msubclicked()
{
    equalClicked();
    sumInMemory-=ui->lineEdit_output->text().toDouble();
}

void calculator::pointClicked()
{
        if (waitingForOperand)
        {
                ui->lineEdit_output->setText("0");
        }
        if (!(ui->lineEdit_output->text().contains(".")))
        {
                ui->lineEdit_output->setText(ui->lineEdit_output->text() + tr("."));
        }
        waitingForOperand = 0;
}

void calculator::change()
{
        QString text = ui->lineEdit_output->text();
        double value = ui->lineEdit_output->text().toDouble();

        if (value > 0)
        {
                text.prepend(tr("-"));
        }
        else  if (value < 0)
        {
                text.remove(0, 1);
        }
        ui->lineEdit_output->setText(text);
}

void calculator::Adsclicked()
{
    double value = ui->lineEdit_output->text().toDouble();
    if (value < 0)
        ui->lineEdit_output->setText(ui->lineEdit_output->text().remove(0, 1));
}

void calculator::DELClicked()
{
//        if (waitingForOperand)
//                return;
        QString text = ui->lineEdit_output->text();
        text.chop(1);       //delete the last number
        if (text.isEmpty())
        {
                text = '0';
                waitingForOperand = 1;
        }
        ui->lineEdit_output->setText(text);
}

void calculator::clearALL()
{
        // if(waitingForOperand)
        //    return;
        SumSoFar = 0;
        FactorSoFar = 0;
        ui->lineEdit_output->setText("0");
        pendingAdditiveOperator.clear();
        pendingMultiplicativeOperator.clear();
        waitingForOperand = 1;
}


void calculator::unaryfunction()
{
        QPushButton *clicked = qobject_cast<QPushButton *>(sender());
        QString operatorname = clicked->text();
        double a = ui->lineEdit_output->text().toDouble();
        double result = 0;
        if (operatorname == tr("x^2"))
        {
                result = pow(a, 2.0);
        }
        else if (operatorname == tr("x^.5"))
        {
                if (a < 0)
                {
                        abortOperation();
                        return;
                }
                else
                {
                        result = sqrt(a);
                }
        }
        else if (operatorname == tr("x^-1"))
        {
                if (a == 0)
                {
                        abortOperation();
                        return;
                }
                result = 1 / a;
        }
        else if (operatorname == tr("x!"))
        {
                result = 1;
                if ((a - (int)a == 0))
                {
                        if (a == 0)
                        {
                                result = 1;

                        }
                        else if (a > 0)
                        {
                                for (int i = a; i >= 1; i--)
                                {
                                        result *= i;
                                }
                        }
                        else
                        {
                                abortOperation();
                                return;
                        }
                }
                else
                {
                        abortOperation();
                        return;
                }
        }
        else if (operatorname == tr("log"))
        {
                if (a <= 0)
                {
                        abortOperation();
                        return;
                }
                else
                {
                        result = log10(a);
                }
        }
        else if (operatorname == tr("ln"))
        {
                if (a <= 0)
                {
                        abortOperation();
                        return;
                }
                else
                {
                        result = log(a);
                }
        }
        else if (operatorname == tr("e^x"))
        {
                result = exp(a);
        }
        else if (operatorname == tr("sin"))
        {
                result = sin(a*pi / 180);
        }
        else if (operatorname == tr("cos"))
        {
                result = cos(a*pi / 180);
        }
        else if (operatorname == QObject::tr("tan"))
        {
                result = tan(a*pi / 180);
        }
        else if (operatorname == QObject::tr("cot"))
        {
                if (tan(a*pi / 180) == 0)
                {
                        abortOperation();
                        return;
                }
                else
                {
                        result = 1 / tan(a*pi / 180);
                }
        }
        else if (operatorname == QObject::tr("arcsin"))
        {
                result = (180 * asin(a)) / pi;
        }
        else if (operatorname == QObject::tr("arccos"))
        {
                result = (180 * acos(a)) / pi;
        }
        else if (operatorname == QObject::tr("arctan"))
        {
                result = (180 * atan(a)) / pi;
        }
        else if (operatorname == QObject::tr("arccot"))
        {
                if (a == 0)
                {
                        abortOperation();
                        return;
                }
                else
                {
                        result = (180 * atan(1 / a)) / pi;
                }
        }
        /*    if(result-(int)result <= 0.00001)
        result = (int)result;
        */
        if (result >= (10 ^ 6))
        {
                ui->lineEdit_output->setText(QString::number(result));
        }
        else
        {
                ui->lineEdit_output->setText(QString::number(result, 'f', 10));
        }
        waitingForOperand = 1;
}


bool calculator::Calculate(double rightOperand, const QString &pendingOperator)
{
        if (pendingOperator == QObject::tr("+"))
        {
                SumSoFar += rightOperand;
        }
        else if (pendingOperator == QObject::tr("-"))
        {
                SumSoFar -= rightOperand;
        }
        else if (pendingOperator == QObject::tr("*"))
        {
                FactorSoFar *= rightOperand;
        }
        else if (pendingOperator == QObject::tr("/"))
        {
                if (rightOperand == 0.0)
                        return 0;
                FactorSoFar /= rightOperand;
        }
        else if (pendingOperator == QObject::tr("x^y"))
        {
                SumSoFar = pow(SumSoFar, rightOperand);
        }
        else if (pendingOperator == QObject::tr("*10^x"))
        {
                SumSoFar = SumSoFar*pow(10, rightOperand);
        }
        return 1;
}

double calculator::getPi()
{
        return pi;
}

void calculator::abortOperation()
{
        clearALL();
        ui->lineEdit_output->setText(QObject::tr("Math ERROR"));
}
