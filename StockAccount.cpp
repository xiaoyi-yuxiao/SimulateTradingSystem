//
//  StockAccount.cpp
//  final_project
//
//  Created by 黄小译 on 11/11/19.
//  Copyright © 2019 黄小译. All rights reserved.
// total port在这里变化，cash持续变化调动cashbal 每次事件产生update total portfolio 在bank里面 这里只调动查看钱，改变钱都在bank操作
#define _CRT_SECURE_NO_WARNINGS
#include "StockAccount.hpp"
#include "BankAccount.hpp"
#include <fstream>
#include <time.h>
#include <iomanip>
using namespace std;
StockAccount::StockAccount(){
    ifstream in("bankCashBalance.txt");
    if (!in.is_open()) {
        CASH_BALANCE=10000;
        ofstream out;
        out.open("bankCashBalance.txt");
        out << setiosflags(ios::fixed) << setprecision(2);
        out<<CASH_BALANCE;
        out.close();
    }
    else{
        double temp;
        ifstream in("bankCashBalance.txt");
        in>>temp;
        CASH_BALANCE=temp;
        in.close();
    }
    ifstream transaction("transactionHistory.txt");
    if (!transaction.is_open()) {
        ofstream out;
        out.open("transactionHistory.txt");
        out << left << setw(10) << "Event" << left << setw(20) << "Company-Symbol" << left << setw(10) << "Number" << left << setw(10) << "Price" << left << setw(15) << "Totalvalue" << left << setw(10) << "Time" << "\n";
        out.close();
    }
    ifstream Portfolioin("portfolio.txt");
    if (!Portfolioin.is_open()) {
        ofstream out;
        out.open("portfolio.txt");
        out<< left << setw(20) << "CompanySymbol" << left << setw(10) << "Number" << left << setw(20) << "Price-per-share" << left << setw(10) << "Totalvalue" << "\n";
    }
    else{
        string firstline;
        Portfolioin>>firstline>>firstline>>firstline>>firstline;//pass the firstline
        string tempname;
        double tempprice;
        int tempshare;
        double temptotal;
        portfolio = new linkedlist;
        while (Portfolioin>>tempname>>tempshare>>tempprice>>temptotal) {
            listnode *newNode = new listnode(tempname,tempprice,tempshare);
            portfolio->addToEnd(newNode);
        }
        Portfolioin.close();
    }
}
double StockAccount::get_market_price(string filename,string stock_name){
    ifstream file(filename);
    string stockSymbol;
    double price;
    string trash;
    while (!file.eof()) {
        file >> stockSymbol;
        if (stockSymbol == stock_name) {
            break;
        }
        file>>price;
        file>>trash;
    }
    if (file.eof()){
        cout << "Fail: There is no such company!" << endl;
        return -1;
    }
    file >> price;
    return price;
}
void StockAccount::display_stock(string filename,string stock_name){
    double price = get_market_price(filename,stock_name);
    if (price!=-1) {
        cout << "Company-symbol price per share" << endl;
        cout << setiosflags(ios::fixed) << setprecision(2);
        cout << left << setw(14)<< stock_name << " $" << price << endl;
    }
}
void StockAccount::display_current_portfolio(){
    getcashbal();
    cout<<"Cash balance = $"<<CASH_BALANCE<<endl;
    string company;
    string num;
    string price;
    string total;
    ifstream in("portfolio.txt");
    while (in>>company>>num>>price>>total) {
        cout<<left << setw(20) << company << left << setw(10) << num << left << setw(20) << price << left << setw(10) << total << "\n";
    }
    cout<<"Total portfolio value: $"<<CASH_BALANCE+portfolio->get_totalVal()<<endl;
}
//include transaction becareful
int StockAccount::buy_shares(string filename, string stock_name, int num_share, double will_to_pay){
    ifstream file(filename);
    string stockSymbol;
    double price;
    string trash;
    int condition;//choose return
    while (!file.eof()) {
        file >> stockSymbol;
        if (stockSymbol == stock_name) {
            break;
        }
        file>>price;
        file>>trash;
    }
    if (file.eof()){
        cout << "Fail: There is no such company!" << endl;
        return -4;
    }
    file >> price;
    getcashbal();//check balance
    if (num_share*price<=CASH_BALANCE&&price<=will_to_pay){//succesfully buy
        if (!portfolio->increase_share(stock_name, num_share)) {
            listnode *newstock = new listnode(stock_name,price,num_share);
            portfolio->addToEnd(newstock);
        }
        int sortchoice;//allow user to choose a sorting method
        cout<<"Choose a sorting method\n";
        cout<<"1. selection sort\n"<<"2. bubble sort\n";
        cout<<"Enter you choice:";
        cin>>sortchoice;
        while (sortchoice<1||sortchoice>2) {
            cout<<"Enter a valid choice";
            cin>>sortchoice;
        }
        if (sortchoice==1) {
            portfolio->sort_list_selection();
        }
        else{
            portfolio->sort_bubble();
        }
        //set the transaction history
        ofstream file;
        file.open("transactionHistory.txt",ios::app);
        //get time;
        time_t seconds;
        seconds = time(NULL);
        char time[12];
        tm * timeinfo;
        timeinfo = localtime(&seconds);
        strftime(time, 100, "%X", timeinfo);
        file << setiosflags(ios::fixed) << setprecision(2);
        double stockValue = price * num_share;
        file << left << setw(10) << "Buy" << left << setw(20) << stock_name << left << setw(10) << num_share<< left << setw(1) << "$" << left << setw(9) << price << left << setw(1) << "$" << left << setw(14) << stockValue << left << setw(10) << time << "\n";
        file.close();
        //update port
        portfolio->updateport();
        //bank transaction， total port updated in bank account;
        BankAccount *bk = new BankAccount();
        bk->withdraw(will_to_pay * num_share);
        delete bk;
        condition=0;
    }
    else if (price>will_to_pay&&num_share*price>CASH_BALANCE){
        condition=-3;
    }
    else if (num_share*price>CASH_BALANCE){
        condition=-2;
    }
    else{
        condition=-1;
    }
    return condition;
}
int StockAccount::sell_shares(string filename, string stock_name, int num_share, double will_to_sell){
    ifstream file(filename);
    string stockSymbol;
    double price;
    string trash;
    int condition=1;//choose return,random initialized;
    while (!file.eof()) {
        file >> stockSymbol;
        if (stockSymbol == stock_name) {
            break;
        }
        file>>price;
        file>>trash;
    }
    if (file.eof()){
        cout << "Fail: There is no such company!" << endl;
        return -2;
    }
    file >> price;
    if (price<will_to_sell){
        return -1;
    }
    if (portfolio->decrease_share(stock_name, num_share)) {
        int sortchoice;//allow user to choose a sorting method
        cout<<"Choose a sorting method\n";
        cout<<"1. selection sort\n"<<"2. bubble sort\n";
        cout<<"Enter you choice:";
        cin>>sortchoice;
        while (sortchoice<1||sortchoice>2) {
            cout<<"Enter a valid choice";
            cin>>sortchoice;
        }
        if (sortchoice==1) {
            portfolio->sort_list_selection();
        }
        else{
            portfolio->sort_bubble();
        }
        //set the transaction history
        ofstream file;
        file.open("transactionHistory.txt",ios::app);
        //get time;
        time_t seconds;
        seconds = time(NULL);
        char time[12];
        tm * timeinfo;
        timeinfo = localtime(&seconds);
        strftime(time, 100, "%X", timeinfo);
        file << setiosflags(ios::fixed) << setprecision(2);
        double stockValue = price * num_share;
        file << left << setw(10) << "SELL" << left << setw(20) << stock_name << left << setw(10) << num_share<< left << setw(1) << "$" << left << setw(9) << price << left << setw(1) << "$" << left << setw(14) << stockValue << left << setw(10) << time << "\n";
        file.close();
        //update port
        portfolio->updateport();
        //bank transaction， total port updated in bank account;
        BankAccount *bk = new BankAccount();
        bk->deposit(will_to_sell * num_share);
        delete bk;
        condition=0;
    }
    else{
        condition=-3;
    }
    return condition;
}
void StockAccount::getcashbal(){
    double temp;
    ifstream in("bankCashBalance.txt");
    in>>temp;
    CASH_BALANCE=temp;
    in.close();
}
void StockAccount::view_transaction_history() {
    char line[100];
    ifstream file;
    file.open("transactionHistory.txt");
    while (!file.eof()) {
        file.getline(line, 200);
        cout << line << endl;
    }
    file.close();
}
double StockAccount::getportval(){
    return portfolio->get_totalVal();
}
