/**
 * GridPoint.cpp
 */

#include "GridPoint.h"
#include "Concentration.h"

#include <iostream>
#include <iterator>
#include <vector> 

using namespace std;

GridPoint::GridPoint(){}

void GridPoint::display() const
{
	cout << "Concentration:{ " << endl;
	for (
		std::vector<Concentration>::const_iterator ita = concentrations.begin(); 
		ita != concentrations.end(); ++ita
	) {
		cout << "\t";
		ita->display();
	}
	cout << "}" << endl;
}

void GridPoint::addConcentration(Concentration concentration)
{
	concentrations.push_back(concentration);
}