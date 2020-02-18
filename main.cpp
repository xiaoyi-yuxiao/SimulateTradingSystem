//
//  main.cpp
//  final_project
//
//  Created by 黄小译 on 11/11/19.
//  Copyright © 2019 黄小译. All rights reserved.
//string stock_Name;
//int Num_share;
//double stock_price;

#include <iostream>
#include <fstream>
#include <string>
#include "listnode.hpp"
#include "linkedlist.hpp"
#include "StockAccount.hpp"
#include "BankAccount.hpp"
using namespace std;
void helper();// this function allow you to reset all the data
void selection_at_begin();//the choice showed at the begining of this program
void StockPortAccount();
void BankAccount_select();
int valid_choice(int,int);//this function is designed to valid the user input to prevent input wrong number
void Stockmanage(int);
void bankmanage(int);
string day1_price = "Result_1.txt";
string day2_price = "Result_2.txt";
int main() {
    int level_1_choice,Port_choice,bank_choice;
    do {
        selection_at_begin();
        level_1_choice = valid_choice(1, 4);
        switch (level_1_choice){
            case 1:
                do {
                    StockPortAccount();
                    Port_choice = valid_choice(1, 7);
                    Stockmanage(Port_choice);
                } while (Port_choice!=7);
                break;
            case 2:
                do {
                    BankAccount_select();
                    bank_choice = valid_choice(1, 5);
                    bankmanage(bank_choice);
                } while (bank_choice!=5);
                break;
            case 3:{helper();break;}
            default:
                break;
        }
    } while (level_1_choice!=4);
 
    return 0;
    // allow user to deposit orwithdraw themselve imprtant
}
void helper(){
    //reset cash
    double CASH_BALANCE=10000;
    ofstream out;
    out.open("bankCashBalance.txt");
    out << setiosflags(ios::fixed) << setprecision(2);
    out<<CASH_BALANCE;
    out.close();
    //resethistory
    ofstream out2;
    out2.open("transactionHistory.txt");
    out2 << left << setw(10) << "Event" << left << setw(20) << "Company-Symbol" << left << setw(10) << "Number" << left << setw(10) << "Price" << left << setw(15) << "Totalvalue" << left << setw(10) << "Time" << "\n";
    out2.close();
    //reset port
    time_t seconds;
    seconds = time(NULL);
    ofstream out3;
    out3.open("portfolio.txt");
    out3<< left << setw(20) << "CompanySymbol" << left << setw(10) << "Number" << left << setw(20) << "Price-per-share" << left << setw(10) << "Totalvalue" << "\n";
    //reset totalport
    ofstream file;
    file.open("totalPorfolioValue.txt");
    file << std::left << setw(15) << "Total Value" << std::left << setw(15) << "Time" << "\n";
    file.close();
    //bank history
    file.open("bankAccountHistory.txt");
    file << left << setw(20) << "Event" << left << setw(20) << "Amount" << left << setw(20) << "Date" << left << setw(20) << "Balance" << "\n";
    file.close();
}

void selection_at_begin(){
    cout<<"Welcome to the Account Management System.\n";
    cout<<"Please select an account to access:\n";
    cout<<"1. Stock Portfolio Account\n2. Bank Account\n3. Reset all value(when you want to reset all the files)\n4. Exit\n\nOption: ";
}
void StockPortAccount(){
    cout<<"Stock Portfolio Account\n\n";
    cout<<"Please select an option\n";
    cout<<"1. Display the price for a stock symbol\n2. Display the current portfolio\n3. Buy shares\n4. Sell shares\n5. View a graph for the portfolio value\n6. View transaction history\n7. Return to previous menu\n\nOption: ";
}
void BankAccount_select(){
    cout<<"Bank Account\n\n";
    cout<<"Please select an option:\n";
    cout<<"1. View account balance\n2. Deposit money\n3. Withdraw money\n4. Print out history\n5. Return to previous menu\n\nOption: ";
}

int valid_choice(int small,int big){
    int choice;
    cin>>choice;
    while (choice<small||choice>big) {
        cout<<"Enter a valid choice: ";
        cin>>choice;
    }
    return choice;
}
void Stockmanage(int option){
    StockAccount Portfolio;
    srand((unsigned)(time(NULL)));
    int fileNo = rand();
    string fileName;
    if (fileNo % 2 == 1) fileName = day1_price;
    else fileName = day2_price;
    switch (option) {
        case 1:{
            string stock;
            cout<<"Please enter the stock symbol: ";
            cin>>stock;
            Portfolio.display_stock(fileName, stock);
            cout<<endl;
            break;
        }
        case 2:{
            Portfolio.display_current_portfolio();
            cout<<endl;
            break;
        }
        case 3:{
            string stock;
            cout << "Please enter the stock symbol you wish to purchase: ";
            cin >> stock;
            int shares;
            cout << "Please enter the number of shares: ";
            cin >> shares;
            double price;
            cout << "Please enter the maximum amount you are willing to pay per share: ";
            cin >> price;

            int outcome = Portfolio.buy_shares(fileName, stock, shares, price);
            switch(outcome){
                case 0:{
                    cout << "You have purchased " <<shares<<" shares of "<<stock<<" at $"<<price<<" each for a total of "<<price*shares<<endl;
                    break;
                }
                case -1:{
                    cout << "Fail: The given price is not satisfied." << endl;
                    break;
                }
                case -2:{
                    cout << "Fail: You do not have enough cash." << endl;
                    break;
                }
                case -3:{
                    cout << "Fail: You do not have enough cash and the given price is not satisfied." << endl;
                    break;
                }
                case -4:{
                    break;
                }
            }
            cout << endl;
            break;
        }
        case 4:{
            string stock;
            cout << "Please enter the stock symbol you wish to sell: ";
            cin >> stock;
            int shares;
            cout << "Please enter the number of shares: ";
            cin >> shares;
            double price;
            cout << "Please enter the maximum amount you are willing to pay per share: ";
            cin >> price;

            int outcome = Portfolio.sell_shares(fileName, stock, shares, price);
            switch(outcome){
                case 0:{
                    cout << "You have selled " <<shares<<" shares of "<<stock<<" at $"<<price<<" each for a total of "<<price*shares<<endl;
                    break;
                }
                case -1:{
                    cout << "Fail: The given price is not satisfied." << endl;
                    break;
                }
                case -2:{
                    cout << "Fail: There is no such company symbol." << endl;
                    break;
                }
                case -3:{
                    cout << "Fail: You do not have enough shares of this stock to sell." << endl;
                    break;
                }
            }
            cout << endl;
            break;
        }
        case 5:{
            //none print graph
            break;
        }
        case 6:{
            Portfolio.view_transaction_history();
            cout<<endl;
            break;
        }
        case 7:{
            break;
        }
        default:
            break;
    }
}
void bankmanage(int option){
    BankAccount bank;
    switch (option) {
        case 1: {
            double cash = bank.get();
            cout << setiosflags(ios::fixed) << setprecision(2);
            cout << "1. You have $" << cash << " in your bank account" << endl;
            break;
        }
        case 2: {
            double Amount;
            cout << "Please select the amount you wish to deposit: $" ;
            cin >> Amount;
            bank.deposit(Amount);
            break;
        }
        case 3: {
            double Amount;
            cout << "Please select the amount you wish to withdraw: $";
            cin >> Amount;
            double outcome = bank.withdraw(Amount);
            if (outcome == -1) {
                cout << "Fail: The cash balance is not enough." << endl;
                cout << endl;
            }
            else {
                cout << "Withdraw successfully!" << endl;
                cout << endl;
            }
            break;
        }
        case 4: {
            bank.printHistory();
            cout << endl;
            break;
        }
        case 5: {
            break;
        }
    }
}
