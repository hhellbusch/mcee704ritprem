#pragma once

/**
 * Purpose: act as a interface to get periodic element objects.
 */

#include "PeriodicElement.h"
#include <string>
#include <map>

#ifdef ENABLE_MEMWATCH
      #include <MemWatch.h>
      #define new    DEBUG_NEW
#endif	// ENABLE_MEMWATCH

class PeriodicElementFactory
{
public:
	PeriodicElementFactory();

public:
	PeriodicElement getElement(std::string symbol);

private:
	void initalizeLookupTable();
	PeriodicElement getElement() const;

private:
	std::map<std::string, PeriodicElement> lookupTable;

};