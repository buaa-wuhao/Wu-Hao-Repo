class InstrumentSpec{
	private:
		unordered_map<string,string> properties;
	public:
		void setProperty(const string& propertyType,const string& property);
		{
			properties[propertyType] = property;
			return;
		}
		string getProperty(const string& propertyType) const
		{
			return properties[propertyType];	
		}
		bool match(const InstrumentSpec& sepc) const
		{
			if(properties.size()!=spec.properties.size())	
				return false;
			for(auto& it:peroperties)
			{
				string key = it.first, val = it.second;
				if(spec.properties.count(key)==0 ||
				   spec.properties[key]!=val)	
					return false;
			}
			return true;
		}
};

class Instrument{
	private:
		double price;
		string serialNO;
		InstrumentSpec i_spec;
	public:
		Instrument(double prc,const string& serial,const InstrumentSpec& spec):price(prc),serialNO(serial),i_spec(spec){}
		void setPrice(double prc);
		void getPrice() const;
		void setSerialNO(string serial);
		void getSerialNO() const;
		InstrumentSpec getSpec() const
		{
			return i_spec;
		}
};
class Inventory{
	private:
		vector<Instrument> instrumentList;
	public:
		Inventory()=default;
		void addInstrument(const Instrument& instru);
		vector<Instrument> search(const InstrumentSpec& spec)
		{
			vector<Instrument> search_res;
			for(int i=0;i<instrumentList.size();i++)	
			{
				if(instrumentList[i].getSpec().match(spec))	
				{
					search_res.push_back(instrumentList[i]);	
				}
			}
			return search_res;
		}

};
