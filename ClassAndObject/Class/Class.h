#pragma once
class Box
{
public:
	double dLength;
	double getWidth(void);
	void setWidth(double wid);

private:
	double dWidth;
	
};


void Box::setWidth(double wid)
{
	dWidth = wid;
}

double Box::getWidth()
{
	return dWidth;
}
