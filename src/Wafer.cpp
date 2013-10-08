/**
 * Wafer.cpp
 */

#include <iterator>
#include <vector> 
#include <iostream> 
#include <math.h>

#include "Wafer.h"
#include "Concentration.h"

using namespace std;

//constructors
Wafer::Wafer(double x, double dx, Concentration initialConcentration)
:_x(x), _dx(dx)
{
	initializeGrid(initialConcentration);
}

// Wafer(double x, double dx, double y, double dy)
// :_x(x), _dx(dx), _y(y), _dy(dy), _z(-1), _dz(-1)
// {

// }

// Wafer (double x, double dx, double y, double dy, double z, double dz)
// :_x(x), _dx(dx), _y(y), _dy(dy), _z(z), _dz(z)
// {

// }

//functions

void Wafer::initializeGrid(Concentration initialConcentration)
{
	//only able to do 1d grids atm
	int numGridPoints = _x / _dx;
	_gridPoints = vector<GridPoint>(numGridPoints);

	//set base concentration
	
	for (
		std::vector<GridPoint>::iterator it = _gridPoints.begin(); 
		it != _gridPoints.end(); ++it
	) {
		it->addConcentration(initialConcentration);
	}

	
}

void Wafer::displayCencentrationToCOUT() const
{
	//display the points
	std::cout << "The contents of _gridPoints are:";
	for (
		std::vector<GridPoint>::const_iterator ita = _gridPoints.begin(); 
		ita != _gridPoints.end(); ++ita
	) {
		ita->display();
	}
	std::cout << '\n';
}


void createPlot()
{
	
}