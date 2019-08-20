#include <string>
#include <iostream>

using namespace std;
int counter = 0;
class sales_data
{
    friend sales_data add(const sales_data& lhs,const sales_data& rhs);	
    friend ostream& print(ostream& os,const sales_data& item);
    friend istream& read(istream& is,sales_data& item);
	
	friend sales_data operator+(const sales_data& lhs,const sales_data& rhs);
	friend ostream&   operator<<(ostream&, const sales_data&);
	friend istream&   operator>>(istream&, sales_data&);
	friend sales_data operator+(const sales_data& lhs,const sales_data& rhs);

	private:
	string bookNO = "";
	unsigned unitsSold = 0;
	double revenue = 0;
	
	public:
	explicit operator string() const {return bookNO;}
	explicit operator double() const {return revenue;}
	sales_data():bookNO(""),unitsSold(0),revenue(0){cout<<"sales_data()"<<endl;}
	sales_data(const string& isbn):bookNO(isbn) {cout<<"sales_data(const string& isbn)"<<endl;}
	sales_data(const string& isbn,unsigned no,double p):bookNO(isbn),unitsSold(no),revenue(p*no) {}
	sales_data(istream& is){ read(is,(*this));}
	sales_data(const sales_data& sd):bookNO(sd.bookNO),unitsSold(sd.unitsSold),revenue(sd.revenue) 
	{
		counter++;
		cout<<"copy constructor "<<counter<<endl;
	}
	sales_data& operator=(const sales_data& rhs) 
	{
		bookNO = rhs.bookNO;
		unitsSold = rhs.unitsSold;
		revenue   = rhs.revenue;
		counter++;
		cout<<"copy assignment "<< counter << endl;
		return(*this);
	}
	~sales_data() {cout<<"~sales_data()"<<endl;}
	sales_data& operator+=(const sales_data&);
	sales_data& operator=(const string& rhs)
	{
		cout<<"sales_data& operator=(const string& rhs)"<<endl;
		bookNO = rhs;
		return (*this);
	}

	string isbn() const {return bookNO;}
	sales_data& combine(const sales_data& rhs);
};

ostream&   operator<<(ostream& os, const sales_data& item)
{
	os<< item.isbn()<<" "<<item.unitsSold <<" "<<item.revenue<<endl;
	return os;
}

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

istream&   operator>>(istream& is, sales_data& item)
{
	is>>item.bookNO>>item.unitsSold>>item.revenue;
	if(is) {}
	else   item = sales_data();
	return is;
}

sales_data& sales_data::combine(const sales_data& rhs)
{
	revenue += rhs.revenue;
	unitsSold += rhs.unitsSold;
	return (*this);
}

sales_data operator+(const sales_data &lhs, const sales_data &rhs)
{
    sales_data sum = lhs;  // copy data members from lhs into sum
     sum.unitsSold += rhs.unitsSold;             // add rhs into sum
     sum.revenue   += rhs.revenue;
	//cout<<"before sum+= rhs"<< endl;
 	//sum += rhs;
 	//cout<<"after sum+= rhs"<< endl;
    return sum;
}

sales_data&  sales_data::operator+=(const sales_data &rhs)
{
   
	cout<<"enter operator+="<< endl;
	(*this) = (*this) + rhs;

    // unitsSold += rhs.unitsSold;
    // revenue += rhs.revenue;
    return (*this);
}



int main()
{
	sales_data sd = string("1234") ;
	//sd = "1234";
	//sales_data sd1("1234");
	//sales_data sd2("1234",2,5);
	//sd2 += sd1;
	cout<<sd<<endl;
	//sd = sd1 + sd2;
	//cout<< sd2 + sd1<<endl;
	//cout<<"end of main"<<endl;
	// sales_data sd3(cin);
	// print(cout,sd);
	// print(cout,sd1);
	// print(cout,sd2);
	// print(cout,sd3);
	// //cout<<sd1.bookNO << "wuhao" << endl;
	// sales_data total(cin);

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



