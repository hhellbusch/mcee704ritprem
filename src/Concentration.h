/**
 * Concentration.h
 */

#pragma once


#include "PeriodicElement.h"
#include "BigIntegerLibrary.hh"

class Concentration
{
public:
	Concentration(PeriodicElement element, BigUnsigned concentration);

public:
	void display() const;

private:
	PeriodicElement element;
	BigUnsigned concentration; //cm^-3
};