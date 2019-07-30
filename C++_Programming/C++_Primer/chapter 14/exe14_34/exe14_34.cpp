
class ifthenelse
{
	int operator()(int cond,int then,int els) const
	{
		if(cond) return then;
		else     return els;
	}
}