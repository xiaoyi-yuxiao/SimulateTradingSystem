//
//  BankAccount.cpp
//  final_project
//
//  Created by 黄小译 on 11/11/19.
//  Copyright © 2019 黄小译. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include "BankAccount.hpp"
#include "StockAccount.hpp"
#include <fstream>
#include <iomanip>
using namespace std;
BankAccount::BankAccount() {
    ifstream in("bankCashBalance.txt");
    if (!in.is_open()) {
        CASH_BALANCE = 10000;
        ofstream file;
        file.open("bankCashBalance.txt");
        file << setiosflags(ios::fixed) << setprecision(2);
        file << 10000 << "\n";
        file.close();

        /*Initialized the bank account history file.*/
        file.open("bankAccountHistory.txt", ios::app);
        file << left << setw(20) << "Event" << left << setw(20) << "Amount" << left << setw(20) << "Date" << left << setw(20) << "Balance" << "\n";
        file.close();
    }
    else {
        double cash;
        ifstream OpenFile("bankCashBalance.txt");
        OpenFile >> cash;
        CASH_BALANCE = cash;
        OpenFile.close();
    }
}
double BankAccount::withdraw(double amount) {
    getcashbal();
    if (amount > CASH_BALANCE) {
        return -1;
    }
    CASH_BALANCE -= amount;
    ofstream file;
    file.open("bankCashBalance.txt");
    file << CASH_BALANCE << "\n";
    file.close();
    file.open("bankAccountHistory.txt", ios::app);
    //calcuate the date.
    time_t seconds;
    seconds = time(NULL);
    char date[12];
    tm * timeinfo;
    timeinfo = localtime(&seconds);
    strftime(date, 100, "%D", timeinfo);
    file << setiosflags(ios::fixed) << setprecision(2);
    file << std::left << setw(20) << "Withdrawal" << std::left << setw(1) << "$" << std::left << setw(19) << amount << std::left << setw(20) << date << std::left << setw(20) << CASH_BALANCE << "\n";
    file.close();
    storeTotalPortfolioValue();
    return amount;
}
void BankAccount::deposit(double amount) {
    getcashbal();
    CASH_BALANCE += amount;
    ofstream file;
    file.open("bankCashBalance.txt");
    file << CASH_BALANCE << "\n";
    file.close();
    file.open("bankAccountHistory.txt", ios::app);
    //calcuate the date.
    time_t seconds;
    seconds = time(NULL);
    char date[12];
    tm * timeinfo;
    timeinfo = localtime(&seconds);
    strftime(date, 100, "%D", timeinfo);
    file << setiosflags(ios::fixed) << setprecision(2);
    file << left << setw(20) << "Deposit" << left << setw(1) << "$" << left << setw(19) << amount << left << setw(20) << date << left << setw(20) << CASH_BALANCE << "\n";
    storeTotalPortfolioValue();
    file.close();
}
void BankAccount::storeTotalPortfolioValue() {
    //calcuate current time.
    time_t seconds;
    seconds = time(NULL);
    StockAccount *sk = new StockAccount();
    double portval = sk->getportval();
    ofstream file("totalPorfolioValue.txt", ios::app);
    if (!file.is_open()) {
        ofstream file;
        file.open("totalPorfolioValue.txt", ios::app);
        file << std::left << setw(15) << "Total Value" << std::left << setw(15) << "Time" << "\n";
        file << setiosflags(ios::fixed) << setprecision(2);
        file << std::left << setw(15) << CASH_BALANCE+portval<< std::left << setw(15) << seconds << "\n";
        file.close();
    }
    else {
        file << setiosflags(ios::fixed) << setprecision(2);
        file << std::left << setw(15) << CASH_BALANCE+portval<< std::left << setw(15) << seconds << "\n";
        file.close();
    }
}
void BankAccount::printHistory() {
    char line[100];
    ifstream inbk("bankCashBalance.txt");
    inbk.getline(line, 100);
    cout << "The cash balance of this account is: " << line << "." << endl;
    cout << endl;

    ifstream in("bankAccountHistory.txt");
    while (!in.eof()) {
        in.getline(line, 100);
        cout << line << endl;
    }
    in.close();
}
void BankAccount::getcashbal(){
    double temp;
    ifstream in("bankCashBalance.txt");
    in>>temp;
    CASH_BALANCE=temp;
    in.close();
}
double BankAccount::get(){
    getcashbal();
    return CASH_BALANCE;
}
