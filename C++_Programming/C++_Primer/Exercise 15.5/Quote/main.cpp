#ifndef QUOTE_H_INCLUDED
#include "Quote.h"
#endif // QUOTE_H_INCLUDED

#include <iostream>
#include <vector>
#include <memory>
#include <set>

using namespace std;


class Basket
{
    private:
    static bool compare(Quote& lhs, Quote& rhs)
    {
        return lhs.isbn() < rhs.isbn();
    }

    multiset<Quote,decltype(compare)*> item{compare};
};


void print_total(ostream &os, const Quote& item, size_t n)
{
    os<<item.net_price(n) << endl;
    return;
}

int main()
{
    cout << "Hello world!" << endl;
    //Disc_Quote dq("dq_book",3.5,8,0.2);
   // Quote quote("book",3.3);
   // quote.debug();
   // Bulk_Quote bq("d_book",3.3,6,0.2);
   // bq.debug();
    //print_total(cout,quote,4);
    //print_total(cout,bq,10);
    vector<shared_ptr<Quote>> basket;
    basket.push_back(make_shared<Bulk_Quote>("12345",50,10,0.25));
    cout<<basket[0]->net_price(15)<<endl;
    return 0;
}
