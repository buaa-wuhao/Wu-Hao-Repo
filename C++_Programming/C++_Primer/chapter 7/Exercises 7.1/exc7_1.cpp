#include <string>
#include <iostream>

using namespace std;
struct sales_data
{
	string bookNO;
	unsigned unitsSold = 0;
	double revenue = 0;

};

int main()
{
	sales_data total;
	if(cin>> total.bookNO >> total.unitsSold >> total.revenue)
	{
		sales_data trans;
		while(cin >> trans.bookNO >> trans.unitsSold >> trans.revenue)
		{
			if(trans.bookNO==total.bookNO)
			{
				total.revenue += trans.revenue;
				total.unitsSold += trans.unitsSold; 
				continue;
			}

			cout << total.bookNO << " " << total.unitsSold << " "<< total.revenue ;
			total = trans;
		}
		cout << total.bookNO << " " << total.unitsSold << " "<< total.revenue;
	}
	else
	{
		cout << "Read Error!" << endl;
		return -1;
	}
	return 0;
}

