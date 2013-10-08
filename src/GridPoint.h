/**
 * GridPoint.h
 */

#pragma once

#include <vector>
#include <string>
#include "PeriodicElement.h"
#include "Concentration.h"


class GridPoint
{
public:
	GridPoint();

public:
	std::vector<Concentration> getConcentrations();
	void addConcentration(Concentration concentration);
	void display() const;

private:
	std::vector<Concentration> concentrations;
};