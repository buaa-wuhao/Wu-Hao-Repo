#include <string>
#include <iostream>

using namespace std;

class sales_data
{
    friend sales_data add(const sales_data& lhs,const sales_data& rhs);	
    friend ostream& print(ostream& os,const sales_data& item);
    friend istream& read(istream& is,sales_data& item);

	private:
	string bookNO = "";
	unsigned unitsSold = 0;
	double revenue = 0;
	
	public:
	// sales_data():bookNO(""),unitsSold(0),revenue(0){}
	// sales_data(const string& isbn):bookNO(isbn) {}
	// sales_data(const string& isbn,unsigned no,double p):bookNO(isbn),unitsSold(no),revenue(p*no) {}
	// sales_data(istream& is){ read(is,(*this));}
	sales_data(string s,int us,double rev):bookNO(s),unitsSold(us),revenue(rev) {cout<<"sales_data(string s,int us,double rev)"<<endl;}
	sales_data():sales_data("",0,0) {cout<<"sales_data()"<<endl;}
	sales_data(string s):sales_data(s,0,0) {cout<<"sales_data(string s)"<<endl;}
	sales_data(istream& is):sales_data() 
	{
		read(is,(*this));
		cout<<"sales_data(istream& is)"<< endl;
	}
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
	sales_data sd("1234",0,1);
	sales_data sd1;
	sales_data sd2("2345");
	sales_data sd3(cin);

	//cout<<sd1.bookNO << "wuhao" << endl;
	//sales_data total(cin);

/*	
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
	}*/
	return 0;
}

