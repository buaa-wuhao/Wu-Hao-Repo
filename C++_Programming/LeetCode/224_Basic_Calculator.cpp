
class Solution {
public:
    int calculate(string s) {
    	if(s.empty()) return 0;
        int pos = 0;
        while(isspace(s[pos])) {pos++;}
        s = "(" + s + ")";        
    	return evaluate(s,pos);
    }

    int evaluate(string& s,int& pos)
    {
    	int i=pos+1;
    	int start;
        int pre_val;        
   		while(isspace(s[i])) {i++;}
        
        if(s[i]!='(') pre_val = parse(s,i);
        else   pre_val = evaluate(s,i);
        while(isspace(s[i])) {i++;}
    	
    	for(;s[i] != ')';i++)
    	{
    		while(isspace(s[i])) {i++;}
    		char oprat  = s[i++];
    		while(isspace(s[i])) {i++;}
    		int opernd2;
    		if(s[i]=='(') 
    		{
    			opernd2 = evaluate(s,i);
    		}
    		else
    		{
    			opernd2 = parse(s,i);
    		}

    		if(oprat=='+') pre_val += opernd2;
    		if(oprat=='-') pre_val  -= opernd2;
    		i--;
    	}
    	pos = i+1;
        //cout<<pre_val<<endl;
    	return pre_val;
    }

    int parse(string& s,int& pos)
    {
    	//cout << pos << endl;
        while(isspace(s[pos])) {pos++;}
    	int start = pos;
    	while(isdigit(s[pos])) {pos++;}
    	string str = s.substr(start,pos-start);
        //cout << s[pos] << " " <<pos << endl;        
        while(isspace(s[pos])) {pos++;}
        return stoi(str);
    }
};

