#include<string>
class HasPtr
{
    private:
        std::string* ps;
        int i;
        int* use;
    public:
        HasPtr(const std::string& s=std::string()):
            ps(new std::string(s)),i(0),use(new int(0)) {++(*use);}
        HasPtr(const HasPtr& hap);
        HasPtr& operator=(const HasPtr& hap);
        ~HasPtr();
};

HasPtr::~HasPtr()
{
    if(--(*use)==0)
    {
        delete ps;
        delete use;
    }
}

HasPtr::HasPtr(const HasPtr& hap)
{
    ps = hap.ps;
    i  = hap.i;
    use = hap.use;
    ++(*use);
}

HasPtr& HasPtr::operator=(const HasPtr& hap)
{
    (*(hap.use))++;
    if(--(*use)==0)
    {
       delete ps;
       delete use;
    }
    ps = hap.ps;
    i  = hap.i;
    use = hap.use;
    return (*this);
}



