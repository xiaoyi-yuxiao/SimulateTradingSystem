//
//  BankAccount.hpp
//  final_project
//
//  Created by 黄小译 on 11/11/19.
//  Copyright © 2019 黄小译. All rights reserved.
//

#ifndef BankAccount_hpp
#define BankAccount_hpp
#include "Account.hpp"
#include <iostream>
class BankAccount:public Account
{
public:
    BankAccount();
    double withdraw(double);
    void storeTotalPortfolioValue();
    void deposit(double);
    void printHistory();
    void getcashbal();
    double get();
};
#endif /* BankAccount_hpp */
