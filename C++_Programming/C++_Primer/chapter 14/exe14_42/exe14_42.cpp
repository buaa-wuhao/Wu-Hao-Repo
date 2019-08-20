#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>


int main()
{
	using std::placeholders::_1;
	std::vector<int> vec = {1,111,1111,11111};

	int count = std::count_if(vec.begin(),vec.end(),std::bind(std::greater<int>(),_1,1024));
	std::cout << count << std::endl;

	std::vector<std::string> svec = {"pooh","pooh","prezy","pooh"};
	auto found = std::find_if(svec.cbegin(),svec.cend(),std::bind(std::equal_to<std::string>(),_1,"pooh"));
	std::cout << *found << std::endl;

	std::transform(vec.begin(),vec.end(),vec.begin(),std::bind(std::multiplies<int>(),_1,2 ));
	for(int i:vec) std::cout<< i << std::endl;
	return 0;


}