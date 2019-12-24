#include "secondary.h"
#include "ui_secondary.h"
#include <iostream>
#include <string>
using namespace std;
secondary::secondary(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::secondary)
{
    ui->setupUi(this);
    string j = "push\n";
    ui->label->setText(QString::fromStdString("plus"));
    for (int i = 0;i<500;i++) {
        if (plus[i]){
           j = j+"+"+plus[i]+" ";
    }}
    j+="\nminus\n";
    for (int i = 0;i<500;i++) {
        if (sub[i]){
           j = j+"-"+sub[i]+" ";
    }}
    ui->label->setText(QString::fromStdString(j));
}

secondary::~secondary()
{
    delete ui;
}
