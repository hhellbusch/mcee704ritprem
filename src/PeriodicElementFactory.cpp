/**
 * PeriodicElementFactory.cpp
 */

#include "PeriodicElementFactory.h"
#include "PeriodicElement.h"
#include <map>
#include <string>


using namespace std;

PeriodicElementFactory::PeriodicElementFactory()
{
	initalizeLookupTable();
}

void PeriodicElementFactory::initalizeLookupTable()
{
	lookupTable = map<string, PeriodicElement> ();
	
	//boron - http://www.webelements.com/boron/
	PeriodicElement boron;
	boron.setFullName("boron");
	boron.setSymbol("B");
	boron.setAtomicWeight(10.811);
	boron.setAtomicNumber(5);

	lookupTable.insert(pair<string, PeriodicElement> ("B", boron));
}

PeriodicElement PeriodicElementFactory::getElement(string symbol)
{
	return lookupTable.at(symbol);
}