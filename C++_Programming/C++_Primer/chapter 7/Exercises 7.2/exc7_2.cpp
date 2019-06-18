#include <string>
#include <iostream>

using namespace std;
struct sales_data
{
	string bookNO;
	unsigned unitsSold = 0;
	double revenue = 0;

	string isbn() const {return bookNO;}
	sales_data& combine(const sales_data& rhs);
};

sales_data add(const sales_data& lhs,const sales_data& rhs)
{
	 sales_data sum = lhs;
	 sum.combine(rhs);
	 return sum;
}

ostream& print(ostream& os,const sales_data& item)
{
	os<<item.isbn()<<" "<<item.unitsSold<<" "<<item.revenue<<endl;
	return os;
}

istream& read(istream& is,sales_data& item)
{
	is>>item.bookNO>>item.unitsSold>>item.revenue;
	return is;
}

sales_data& sales_data::combine(const sales_data& rhs)
{
	revenue += rhs.revenue;
	unitsSold += rhs.unitsSold;
	return (*this);
}

struct Person {
	string name;
	string addr;
	string mingzi() const {return name;}
	string dizhi()  const {return addr;}
};

istream& p_read(istream& is,Person& item)
{
	is >> item.name >> item.addr;
	return is;
}
 ostream& p_print(ostream& os,const Person& item)
 {
 	os << item.name << " " << item.addr << endl;
 	return os;
 }

int main()
{
	sales_data total;
	if(read(cin,total))
	{
		sales_data trans;
		while(read(cin,trans))
		{
			if(trans.isbn()==total.isbn())
			{
				total.combine(trans); 
				continue;
			}

			//cout << total.isbn() << " " << total.unitsSold << " "<< total.revenue ;
			print(cout,total);
			total = trans;
		}
		//cout << total.isbn() << " " << total.unitsSold << " "<< total.revenue;
		print(cout,total);
	}
	else
	{
		cout << "Read Error!" << endl;
		return -1;
	}
	return 0;
}

