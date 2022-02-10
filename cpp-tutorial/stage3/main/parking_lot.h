
class parkingSpot{
	public:
		parkingSpot(int id,int sz,int x, int y):spot_id(id),spot_sz(sz),spot_taken(false),
		pos_x(x),pos_y(y) {}
		void clearParkingSpot();
		void takeParkingSpot();
		bool isParkingSpotTaken();
		size_t getParkingSpotSize();
		virtual ~parkingSpot() = default;
	private:
		int spot_id;
		int spot_sz;
		bool spot_taken;
		double pos_x;
		double pos_y;
};

class busParkingSpot:public parkingSpot{
	public:
		busParkingSpot(int id):parkingSpot(id,3){}
};

class carParkingSpot:public parkingSpot{
	public:
		carParkingSpot(int id):parkingSpot(id,2){}
};

class motorParkingSpot:public parkingSpot{
	public:
		motorParkingSpot(int id):parkingSpot(id,1){}
};

struct ticket{
	int ticket_id;
	int spot_id;
	int issue_time;
};

class parkingSpotSortingStrategy{
	public:
		bool operator()(const parkingSpot* lhs,const parkingSpot& rhs){
				
		}
	private:
		double ref_x;
		double ref_y;
};

class parkingLot{
	public:
		//when enter the parkinglot,print the ticket
		ticket printTicket(int gateNum,size_t vehicle_sz);
		//when leave the parkinglot,scan the ticket and calculate the fee
		double scanAndCalculate(const ticket& t);


	private:
		unordered_map<int,parkingSpot*> spots_database;
		unordered_map<int,ticket> tickets_databaze;
		unordered_set<int> spots_taken;
		int num_of_gates;
		vector<priority_queue<parkingSpot*,vector<parkingSpot*>,parkingSpotSortingStrategy>> vq;
		int assignParkingSpot(int gateNum,size_t vehicle_sz);


};
