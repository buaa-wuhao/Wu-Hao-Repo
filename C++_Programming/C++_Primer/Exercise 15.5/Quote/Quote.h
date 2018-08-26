#ifndef QUOTE_H_INCLUDED
#define QUOTE_H_INCLUDED
#include <string>
class Quote{

public:
    Quote()=default;
    Quote(const std::string& book,double sale_price):bookNo(book),price(sale_price){};
    std::string isbn() const {return bookNo;}
    virtual double net_price(std::size_t n) const
    {
        return n*price;
    }
    virtual ~Quote()=default;


protected:
    double price = 0.0;
private:
    std::string bookNo;


};


#endif // QUOTE_H_INCLUDED
