#include <iostream>
#include <vector>
#include <string>
#include "gapbuffer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondary.h"
using namespace std;
const int M = 4020;
const int N = 2025;

char add[500];int lenadd = 0;
char sub[500];int lensb = 0;
int lookup[M][N];

void difference_finder(gap_buffer one, gap_buffer two,int m,int n){
    //m is size of gap buffer one, and n is size of gap buffer two
    //one.size and two.size are passed as arguements for this
     lenadd = 0;
     lensb = 0;
    string X = one.convertToString();
    string Y = two.convertToString();
    if ((m > 0 && n > 0) && X[m-1] == Y[n-1]){
        difference_finder(one,two,m-1,n-1);
    }

    else if (n > 0 && (m==0 || lookup[m][n-1] >= lookup[m-1][n])){
        difference_finder(one,two,m,n-1);
        add[lenadd] = X[m-1];lenadd++;
    }

    else if (m > 0 && (n==0 || lookup[m][n-1] < lookup[m-1][n])){
        difference_finder(one,two,m-1,n);
        sub[lensb] = Y[m-1];lensb++;
    }
}
QString currentFile;
gap_buffer file1;
gap_buffer file2;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open the File");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't open this file : " + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    for (int i = 0; i < 10; i++) {
           file1.buffer.push_back('_');
    }
    std::string current_locale_text = text.toLocal8Bit().constData();
    file1.insert(current_locale_text,0);
    std::string idk =  file1.convertToString();
    QString ddk = QString::fromStdString(idk);
    ui->textEdit->setText(ddk);

    file.close();
    start();
}
void MainWindow::on_actioncompare_with_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open the File");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't open this file : " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    for (int i = 0; i < 10; i++) {
           file2.buffer.push_back('_');
    }
    std::string current_locale_text = text.toLocal8Bit().constData();
    file2.insert(current_locale_text,0);
    difference_finder(file1,file2,file1.size,file2.size);
    sec = new secondary(this);
    for (int i = 0;i<lenadd;i++) {
        sec->plus[i] = add[i];
    }
    for (int i = 0;i<lensb;i++) {
        sec->sub[i] = sub[i];
    }
    sec->show();
}
void MainWindow::update(){
    QString text = ui->textEdit->toPlainText();
    for(int x = 0;x<=(file1.size);x++){
        file1.delete_character(x);
    }
    for (int i = 0; i < 10; i++) {
           file1.buffer.push_back('_');
    }
    std::string current_locale_text = text.toLocal8Bit().constData();
    file1.insert(current_locale_text,0);
    //std::string idk =  file1.convertToString();
    //QString ddk = QString::fromStdString(idk);
    //ui->textEdit->setText(ddk);


}
void MainWindow::start(){
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}
void MainWindow::on_actionnew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void MainWindow::on_actionsave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save As");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't save this file : " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.close();
}
void MainWindow::on_actionsave_2_triggered(){
    return;
};

void MainWindow::on_actionsave_triggered()
{
    QString filename = windowTitle();
    QFile file(filename);
    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't save this file : " + file.errorString());
        return;
    }
    currentFile = filename;
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out<<text;
    file.close();
}


void gap_buffer::grow(int k, int position)
{

    char copy[size];
    for (int i = position; i < size; i++) {
        copy[i - position] = buffer[i];

    }

    for (int i = 0; i < k; i++) {
        buffer.insert(buffer.begin() + i + position, '_');
    }

    for (int i = 0; i < k + position; i++) {
        buffer.insert(buffer.begin()+ position + i + k,copy[i]);
    }

    size += k;
    gap_right+=k;
}

void gap_buffer::left(int position)
{

    while (position < gap_left) {
        gap_left--;
        gap_right--;
        buffer.at(gap_right+1) = buffer[gap_left];
        buffer.at(gap_left)='_';
    }
}

void gap_buffer::right(int position){
    while (position > gap_left) {
        gap_left++;
        gap_right++;
        buffer.at(gap_left-1) = buffer[gap_right];
        buffer.at(gap_right)='_';
    }
}

void gap_buffer::move_cursor(int position)
{
    if (position < gap_left) {
        left(position);
    }
    else {
        right(position);
    }
}

void gap_buffer::insert(string input, int position){
    int len = input.length();
    int i = 0;

    if (position != gap_left) {
        move_cursor(position);
    }

    while (i < len) {
        if (gap_right == gap_left) {
            int k = 10;
            grow(k, position);

        }
        buffer.at(gap_left) = input[i];
        gap_left++;
        i++;
        position++;
    }
}

void gap_buffer::delete_character(int position){
    if (gap_left != position + 1){
        move_cursor(position + 1);
        }
    gap_left--;
    buffer.at(gap_left) = '_';
}




void lookup_table_filler(string X, string Y, int m, int n){

    for (int i = 0; i <= m; i++){
        lookup[i][0] = 0;
    }

    for (int j =0; j<=n; j++){
        lookup[0][j] = 0;
    }

    for (int i = 1; i<=m; i++){
        for (int j = 1; j<=n; j++){
            if (X[i - 1] == Y[j - 1]){
                lookup[i][j] = lookup[i - 1][j - 1] + 1;
            }
            else{
                lookup[i][j] = max(lookup[i - 1][j],lookup[i][j - 1]);
            }
        }
    }
}

string gap_buffer::convertToString(){
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        if (buffer[i]!='_')
            s = s + buffer[i];
    }
    return s;
}



vector<string> split(const string& s)
{
    vector<string> ret;
    typedef string::size_type string_size;
    string_size i = 0;
    // invariant: we have processed characters [original value of i, i)
    while (i != s.size()) {
        // ignore leading blanks
        // invariant: characters in range [original i, current i) are all spaces
        while (i != s.size() && isspace(s[i]))
            ++i;

        // find end of next word
        string_size j = i;
        // invariant: none of the characters in range [original j, current j)is a space
        while (j != s.size() && !isspace(s[j]))
            j++;

        // if we found some nonwhitespace characters
        if (i != j) {
            // copy from s starting at i and taking j - i chars
            ret.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return ret;
}

void searchAndReplace(string s,string old_value,string new_value) {


    vector<string> v = split(s);
    replace(v.begin(), v.end(), old_value, new_value);
    return ;
}
