#pragma once

/**
 * Author: Henry Hellbusch
 * Date: 10/5/2013
 */

#include <string>

#ifdef ENABLE_MEMWATCH
      #include <MemWatch.h>
      #define new    DEBUG_NEW
#endif	// ENABLE_MEMWATCH

class PeriodicElement
{
public: 
	PeriodicElement();
public:
	void setFullName(std::string name);
	void setSymbol(std::string symbol);
	void setAtomicWeight(double atomicWeight);
	void setAtomicNumber(int atomicNumber);
	
public:
	std::string getName() const;
	std::string getSymbol() const;
	double getAtomicWeight() const;
	int getAtomicNumber() const;

private:
	std::string _name;
	std::string _symbol;
	int _atomicNumber;
	double _atomicWeight;
};