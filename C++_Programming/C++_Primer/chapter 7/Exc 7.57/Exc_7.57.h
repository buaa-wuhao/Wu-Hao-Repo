class Account
{
public:
	doulb calculate() {amount += amount*interestRate;}
	static double rate() {return interestRate;}
	static void rate(double r) {interestRate = r;}
private:
	static double interestRate;
	double amount;

};

double Account::interestRate = 0;
double Account::rate() 