class ParkingSpot{
	size_t spotID;
	bool   taken;
};

class Vehicle{
	Vehicle(const string& licence, size_t sz):licenceID(licence),vehicleSize(sz){}
	public:
		string getID(){
			return licenceID;
		}
		size_t getSize(){
			return vehicleSize;	
		}
	private:
		string licenceID;
		size_t vehcileSize;
};

class Bus : public Vehicle{
	Bus(const string& licence):Vehicle(licence,3){}
};
class Car : public Vehicle{
	Car(const string& licence):Vehicle(licence,2){}
};
class Motor:public Vehicle{
	Motor(const string& licence):Vehicle(licence,1){}
};
struct Ticket{
	size_t ticketID;
	ParkingSpot spot;
	Vehicle     veh;
};

class ParkingLot{

	public:
		int getAvailableCnt(){
			return availableCnt;	
		}
		Ticket parkVehicle(const Vehicle& veh){
		
		}
	 	void clearVehicle(const Ticket& ticket){
				
		};	
	private:
		int availableCnt;
		unordered_map<size_t,ParkingSpot> spots;
		unordered_map<size_t,Ticket> tickets;
};
