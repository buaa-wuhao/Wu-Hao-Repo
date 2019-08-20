#include <iostream>
#include <string>
using namespace std;

class Quote
{
	public:
	Quote() = default;
	Quote(const string& book, double sale_price):bookNo(book),price(sale_price) {}
	string isbn() const {return bookNo;}
	virtual double netprice(size_t num) const{return num*price;} 
	virtual ~Quote() = default;
	private:
		string bookNo;
	protected:
		double price = 0.0;
};

class bulk_Quote:public Quote{
	public:	
	bulk_Quote() = default;
	bulk_Quote(const string& book,double sale_price,size_t mqty,double disct):
	Quote(book,sale_price),min_qty(mqty),discount(disct) {}
	double netprice(size_t num) const override
	{
		if(num>min_qty) return price*(1-discount)*num;
		else return price * num;
	}
	private:
	size_t min_qty = 0;
	double discount  =0.0;
};

void print_total(ostream& os, Quote& qt,size_t n)
{
	os<<qt.isbn() << " " << qt.netprice(n) << endl;
}

int main()
{
	Quote qt("wuhao",5);
	bulk_Quote bqt("wuhao28",4,4,0.2);
	print_total(cout,qt,5);
	print_total(cout,bqt,5);
	return 0;
}