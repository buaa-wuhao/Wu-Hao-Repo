
struct destination;
struct connection;

connection connect(destination*);
void disconnect(connection);
void endConnection(connection* p)
{
	disconnect(*p);
}
void f(destination &d)
{
	connection c = connect(&d);
	shared_ptr<connection> = p(&c,endConnection);
	shared_ptr<connection> = p(&c,[](connection* p){disconnect(*p)});
}