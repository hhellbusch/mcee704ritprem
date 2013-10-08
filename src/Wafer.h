#pragma once

/**
 * Encapsulates the simulation grid.
 */

#include <vector>
#include "GridPoint.h"
#include "Concentration.h"

#ifdef ENABLE_MEMWATCH
      #include <MemWatch.h>
      #define new    DEBUG_NEW
#endif	// ENABLE_MEMWATCH



class Wafer
{
public:
	//initializes a 1d wafer
	Wafer(double x, double dx, Concentration initialConcentration);

	// //initializes a 2d wafer
	// Wafer(double x, double dx, double y, double dy);

	// //initializes a 3d wafer
	// Wafer (double x, double dx, double y, double dy, double z, double dz);


public:
	void displayCencentrationToCOUT() const;
	void createPlot();

private:
	void initializeGrid(Concentration baseConcentration);


private:
	double _x;
	double _dx;
	// double _y;
	// double _dy;
	// double _z;
	// double _dz;
	std::vector<GridPoint> _gridPoints;

};