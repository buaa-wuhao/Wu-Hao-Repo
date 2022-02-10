
struct PIDController{
	double kp;
	double kd;
	double ki;
	double Iterm;
	double prvError;
	double maxIterm;
	double minIterm;

	PIDController(){
		kp = 0;
		kd = 0;
		ki = 0;
		Iterm =0;
		prvError = 0;	
		maxIterm = 0;
		minIterm = 0;
	}
	double pidUpdate(double error)
	{
		Iterm += error;
		if(Iterm > maxIterm)	
			Iterm = maxIterm;
		else if(Iterm < minIterm)
			Iterm = minIterm;

		double Dterm = error - prvError;
		prvError = error;

		return kp*error + ki*Iterm + kd*Dterm;
	}
};
