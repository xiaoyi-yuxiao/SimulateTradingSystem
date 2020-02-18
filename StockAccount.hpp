//
//  StockAccount.hpp
//  final_project
//
//  Created by 黄小译 on 11/11/19.
//  Copyright © 2019 黄小译. All rights reserved.
//

#ifndef StockAccount_hpp
#define StockAccount_hpp
#include "Account.hpp"
#include "linkedlist.hpp"
#include <iostream>
class BankAccount;
class StockAccount:public Account
{
    friend class BankAccount;
public:
    StockAccount();
    double get_market_price(string filename,string stock_name);
    void display_stock(string filename,string stock_name);
    void display_current_portfolio();
    int buy_shares(string filename,string stock_name,int num_share, double will_to_pay);
    int sell_shares(string filename,string stock_name,int num_share,double will_to_sell);
    void view_graph();
    void view_transaction_history();
    void getcashbal();//check balance everytime
    double getportval();
private:
    linkedlist *portfolio;
};
#endif /* StockAccount_hpp */
