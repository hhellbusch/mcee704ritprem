/**
 * Author: Henry Hellbusch <hhellbusch@gmail.com>
 */

#include <string>
#include "PeriodicElement.h"

using namespace std;

//constructors
PeriodicElement::PeriodicElement()
{

}

//setters
void PeriodicElement::setFullName(string name)
{
	_name = name;
}

void PeriodicElement::setSymbol(string symbol)
{
	_symbol = symbol;
}

void PeriodicElement::setAtomicWeight(double atomicWeight)
{
	_atomicWeight = atomicWeight;
}

void PeriodicElement::setAtomicNumber(int atomicNumber)
{
	_atomicNumber = atomicNumber;
}

//getters
string PeriodicElement::getName() const
{
	return _name;
}

string PeriodicElement::getSymbol() const
{
	return _symbol;
}

double PeriodicElement::getAtomicWeight() const 
{
	return _atomicWeight;
}

int PeriodicElement::getAtomicNumber() const 
{
	return _atomicNumber;
}