#include <string>
#include <iostream>

using namespace std;

struct sales_data;

istream& read(istream& is,sales_data& item);

struct sales_data
{
	string bookNO = "";
	unsigned unitsSold = 0;
	double revenue = 0;
	sales_data():bookNO(""),unitsSold(0),revenue(0){}
	sales_data(const string& isbn):bookNO(isbn) {}
	sales_data(const string& isbn,unsigned no,double p):bookNO(isbn),unitsSold(no),revenue(p*no) {}
	sales_data(istream& is){ read(is,(*this));}

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



int main()
{
	sales_data sd;
	sales_data sd1("1234");
	sales_data sd2("1234",2,5);
	sales_data sd3(cin);
	print(cout,sd);
	print(cout,sd1);
	print(cout,sd2);
	print(cout,sd3);

	sales_data total(cin);
	if(!total.bookNO.empty())
	{
		sales_data trans(cin);
		while(!trans.bookNO.empty())
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

