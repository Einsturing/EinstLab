#include "widget.h"
#include "ui_widget.h"
#include <string>
#include <cmath>
#include <stack>

#include <QtMath>
#include <QDebug>
#include <QVector>
#include <QCompleter>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label_3->hide();
    ui->label_4->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->qwtPlot->setAutoReplot(true);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"x轴");
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"y轴");
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-2.0,2.0);
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,-2.0,2.0);
    ui->qwtPlot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
    grid=new QwtPlotGrid;
    grid->setMajorPen(QColor(0,255,0,100),0,Qt::DotLine);
    grid->attach(ui->qwtPlot);
    mk1=new QwtPlotMarker;
    mk2=new QwtPlotMarker;
    mk1->setLineStyle(QwtPlotMarker::VLine);
    mk2->setLineStyle(QwtPlotMarker::HLine);
    mk1->setLinePen(QPen(Qt::green,1));
    mk2->setLinePen(QPen(Qt::green,1));
    mk1->setXValue(0);
    mk2->setYValue(0);
    mk1->attach(ui->qwtPlot);
    mk2->attach(ui->qwtPlot);
    (void) new QwtPlotMagnifier(ui->qwtPlot->canvas());
    (void) new QwtPlotPanner(ui->qwtPlot->canvas());
    QStringList funcList;
    funcList<<"sin()"<<"cos()"<<"tg()"<<"log()"<<"x^2";
    QCompleter* completer=new QCompleter(funcList,this);
    ui->lineEdit->setCompleter(completer);
}

Widget::~Widget()
{
    delete ui;
}

int count_(int n)
{
    int count = 0;
    while (n != 0)
    {
        n /= 10;
        count++;
    }
    return count;
}

bool Widget::priority(char lhs, char rhs)
{
    if (rhs == '(')
        return false;
    if (lhs == '+' || lhs == '-')
        return true;
    if ((lhs == '*' || lhs == '/') && (rhs!='+'||rhs!='-'))
        return true;
    if ((lhs == '^') && (rhs == 's' || rhs == 'c' || rhs == 't' || rhs == 'l'||rhs=='.'))
        return true;
    return false;
}

string Widget::inPrefix2postPrefix(string str)
{
    string res;//后缀表达式结果
    stack<char> s;
    for (int i = 0; i<str.length(); i++)
    {
        //如果是数字或自变量，直接加入后缀表达式结果中
        if (isdigit(str[i])||str[i]=='x'||str[i]=='X'||str[i]=='u')
        {
            while (i<str.length() && (isdigit(str[i]) || str[i] == 'x' || str[i] == 'X' || str[i] == 'u'))
            {
                res += str[i];
                i++;
            }
            i--;//注意这里要将i减1，因为上面的循环将i多右移了一位，如果不减1，会漏掉一位
            res += " ";
        }
        else //如果是符号，需要与栈顶的元素进行比较
        {
            //如果栈为空，将其直接压入栈中;如果是左括号(，也直接将其压入栈中
            if (s.empty() || str[i] == '(')
            {
                s.push(str[i]);
                if (str[i] == 's' || str[i] == 'c' || str[i] == 'l' || str[i] == 't')
                    i += 2;
            }
            else
            {
                //当碰到右括号时，将栈中的数据出栈，直到碰到左括号，注意左右括号都不需要加入结果res中
                if (str[i] == ')')
                {
                    while (!s.empty() && s.top() != '(')//注意在对栈执行top操作之前需要判断栈是否为空
                    {
                        res += s.top();
                        res += " ";
                        s.pop();
                    }
                    s.pop();
                }
                else
                {
                    //此时表示该字符为符号，并且不为'('和')'
                    if (priority(str[i], s.top()))//如果它的优先级不高于栈顶符号，那么将栈顶符号出栈
                    {

                        while (!s.empty() && priority(str[i], s.top()))
                        {
                            res += s.top();
                            res += " ";
                            s.pop();
                        }
                        s.push(str[i]);//最后记得将该符号入栈
                        if (str[i] == 's' || str[i] == 'c' || str[i] == 'l' || str[i] == 't')
                        {
                            i += 2;
                        }
                    }
                    else //如果它的优先级比栈顶符号高
                    {
                        s.push(str[i]);
                        if (str[i] == 's' || str[i] == 'c' || str[i] == 'l' || str[i] == 't')
                        {
                            i += 2;
                        }
                    }
                }
            }
        }
    }
    while (!s.empty())//遍历完字符串后将栈中剩余的元素加入结果集中
    {
        res += s.top();
        res += " ";
        s.pop();
    }
    return res;
}

double Widget::operate(double first, double second, char op)
{
    double res = 0;
    switch (op)
    {
    case '+':
        res = first + second;
        break;
    case '-':
        res = first - second;
        break;
    case '*':
        res = first * second;
        break;
    case '/':
        res = first / second;
        break;
    case '^':
        res = pow(first, second);
        break;
    case '.':
        res = first + second / pow(10,count_(second));
        break;
    default:
        break;
    }
    return res;
}

double Widget::operate(double x, char op)
{
    double res = 0;
    switch (op)
    {
    case 's':
        res = sin(x);
        break;
    case 'c':
        res = cos(x);
        break;
    case 'l':
        res = log(x);
        break;
    case 't':
        res = tan(x);
        break;
    case '-':
        res = -x;
        break;
    case '+':
        res = x;
        break;
    default:
        break;
    }
    return res;
}

double Widget::calculateByPostPrefix(string input, double x)
{
    stack<double> s;
    int tmp = 0;
    for (int i = 0; i<input.length(); i++)
    {
        if (isdigit(input[i]))//如果遇到的是数字，就将数字入栈
        {
            while (i<input.length() && isdigit(input[i]))
            {
                tmp = 10 * tmp + input[i] - '0';
                i++;
            }
            //得到数字以后将这个输入压入栈中
            s.push(tmp);
            i--;
        }
        else if (input[i] == 'x' || input[i] == 'X' || input[i] == 'u')//如果遇到的是变量，就将变量入栈
        {
            s.push(x);
        }
        else if (input[i] == ' ')//如果遇到空格，就将tmp重置为0
            tmp = 0;
        else//此时遇到的就是符号
        {
            if (input[i] == 's' || input[i] == 'c' || input[i] == 't' || input[i] == 'l')
            {
                double x = s.top();
                double local = operate(x, input[i]);
                s.push(local);
            }
            else
            {
                double second = s.top();
                s.pop();
                if (s.empty())
                {
                    double local = operate(second, input[i]);
                    s.push(local);
                }
                else
                {
                    double first = s.top();
                    s.pop();
                    double local = operate(first, second, input[i]);
                    s.push(local);
                }
            }
        }
    }
    return s.empty() ? 0 : s.top();
}

void Widget::on_pushButton_clicked()
{
    xs.clear();
    ys.clear();
    if(ui->range_l->text().isEmpty()||ui->range_r->text().isEmpty()||(ui->range_r->text().toDouble()<ui->range_l->text().toDouble()))
    {
        QMessageBox::critical(NULL, "错误","错误的定义域", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    if(flag==1)
    {
        string str=ui->lineEdit->text().toStdString();
        string ot=inPrefix2postPrefix(str);
        for(double i=ui->range_l->text().toDouble();i<=ui->range_r->text().toDouble();i+=0.01)
        {
            xs.append(i);
            ys.append(calculateByPostPrefix(ot,i));
        }
        curve.attach(ui->qwtPlot);
        curve.setSamples(xs,ys);
        curve.setStyle(QwtPlotCurve::Lines);
        curve.setCurveAttribute(QwtPlotCurve::Fitted,true);
        curve.setPen(QPen(Qt::green));
        curve.setRenderHint(QwtPlotItem::RenderAntialiased,true);
    }
    else if(flag==-1)
    {
        string strx=ui->lineEdit_2->text().toStdString();
        string stry=ui->lineEdit_3->text().toStdString();
        string otx=inPrefix2postPrefix(strx);
        string oty=inPrefix2postPrefix(stry);
        for(double i=ui->range_l->text().toDouble();i<=ui->range_r->text().toDouble();i+=0.01)
        {
            xs.append(calculateByPostPrefix(otx,i));
            ys.append(calculateByPostPrefix(oty,i));
        }
        curve.attach(ui->qwtPlot);
        curve.setSamples(xs,ys);
        curve.setStyle(QwtPlotCurve::Lines);
        curve.setCurveAttribute(QwtPlotCurve::Fitted,true);
        curve.setPen(QPen(Qt::green));
        curve.setRenderHint(QwtPlotItem::RenderAntialiased,true);
    }
}

void Widget::on_pushButton_3_clicked()
{
    flag*=-1;
    if(flag==1)
    {
        ui->label->show();
        ui->lineEdit->show();
        ui->label_3->hide();
        ui->lineEdit_2->hide();
        ui->label_4->hide();
        ui->lineEdit_3->hide();
    }
    else if(flag==-1)
    {
        ui->label->hide();
        ui->lineEdit->hide();
        ui->label_3->show();
        ui->lineEdit_2->show();
        ui->label_4->show();
        ui->lineEdit_3->show();
    }
}

void Widget::on_pushButton_4_clicked()
{
    isgrid*=-1;
    if(isgrid==1)
    {
        ui->pushButton_4->setText("隐藏网格");
        grid->show();
    }
    else if(isgrid==-1)
    {
        ui->pushButton_4->setText("显示网格");
        grid->hide();
    }
}
