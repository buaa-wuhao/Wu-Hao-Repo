class Solution {
public:
int calculate(string s) {
	if(s.empty()) return 0;	
	s = '+' + s;	

    int i = 0;
	int start = 1;
	char oprt;
	int oprn1 =0, pre_val = 0;
	int pre_oprn1 = 0;
	
	while(i < s.size())
	{
		if(s[i]=='+' || s[i]=='-' ||
		   s[i]=='*' || s[i]=='/')
		{
			oprt = s[i++];
		}

		while(i<s.size() && s[i]!='+' &&
			  s[i]!='-'  && s[i]!='*' && s[i]!='/')
		{
			i++;
		}
        //cout<<s.substr(start,i-start)<<endl;
		oprn1 = stoi(s.substr(start,i-start));
       // cout<< oprn1 << endl;
        start = i + 1;
        
		if(oprt=='+')
		{
			pre_val += oprn1;
			pre_oprn1 = oprn1;
		}
		else if(oprt=='-')
		{
			pre_val -= oprn1;
			pre_oprn1 = -oprn1;
		}
		else if(oprt=='*')
		{
			pre_val = pre_val-pre_oprn1 + pre_oprn1*oprn1;
			pre_oprn1 *= oprn1;
		}
		else
		{
			pre_val = pre_val-pre_oprn1 + pre_oprn1 / oprn1;
			pre_oprn1 /= oprn1;
		}

	}
	return pre_val;
}

























};