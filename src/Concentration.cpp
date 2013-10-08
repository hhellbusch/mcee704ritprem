/**
 * Concentration.cpp
 */

#include "Concentration.h"
#include "PeriodicElement.h"
#include <iostream>

using namespace std;

Concentration::Concentration(
	PeriodicElement element, 
	BigUnsigned concentration
):element(element), concentration(concentration){}

void Concentration::display() const
{
	cout << "{concentration: " << concentration 
		<< ", name:" << element.getName() << "}" << endl;
}