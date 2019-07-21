#ifndef QUOTE_H_INCLUDED
#define QUOTE_H_INCLUDED
#include <string>
#include <iostream>
class Quote{

public:
    Quote()=default;
    Quote(const std::string& book,double sale_price):bookNo(book),price(sale_price){};
    std::string isbn() const {return bookNo;}
    virtual double net_price(std::size_t n) const
    {
        return n*price;
    }
    virtual void debug()
    {
        std::cout<<"price: "<<price << " bookNo: "<<bookNo<<std::endl;
    }
    virtual ~Quote()=default;


protected:
    double price = 0.0;
private:
    std::string bookNo;


};

class Disc_Quote: public Quote
{
public:
    Disc_Quote() = default;
    Disc_Quote(const std::string& book,double sale_price,int qty,double disc):
        Quote(book,sale_price),min_qty(qty),discount(disc) {}
    double net_price(std::size_t n) const=0;
    void  debug() override
    {
        Quote::debug();
        std::cout<<" min_qty "<<min_qty << " discount " << discount<<std::endl;
    }

protected:
    int min_qty = 0;
    double discount = 0.0;
};

class Bulk_Quote:public Disc_Quote{
public:
    Bulk_Quote() = default;
    Bulk_Quote(const std::string& book,double sale_price,int qty,double disc):
        Disc_Quote(book,sale_price,qty,disc){}

    double net_price(std::size_t n) const override
    {

        if(n>min_qty)
        {
             //std::cout<<"override" << min_qty <<" "<<discount<< std::endl;
            return price * (1-discount) * n;
        }
        else
        {
            return price * n;
        }
    }
};

#endif // QUOTE_H_INCLUDED
