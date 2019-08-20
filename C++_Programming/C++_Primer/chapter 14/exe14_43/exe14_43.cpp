#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;
int main()
{
	vector<int> vec = {2,3,4,5};
	int input;
	cin>>input;
	auto predicator = [&](int i){return 0==std::modulus<int>()(input,i);};
	bool is_divisible = std::any_of(vec.begin(),vec.end(),predicator);
	cout << is_divisible << endl;
	return 0; 
}