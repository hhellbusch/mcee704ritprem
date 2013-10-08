/**
 * Purpose: To create a simple implementation of SURPREM
 * SUPREM stands for Standford University Process Emulation Module.
 * As such, this project has been named RITPREM for Rochester
 * Institute of Technology Process Emulation Module.
 *
 * Assignment Levels:
 * Level 1:
 *  - Model a predep process from a constant-source 
 *  	(fixed surface concentration)
 *  - Model an implant profile with specified energy and dose
 *  	(gaussian profile)
 *  - Redistribute the predep/implant profile - constant dose 
 *  	drive-in (capped surface)
 *  - Assume a constant D diffusion model
 *  - Extracts dose, junction depth, and sheet resistance
 *
 * Level 2:
 *  - Accommodate a constant D or fermi diffusion model
 *  - Accomodate dopant segregation between silicon and oxide
 *  - Model a predep from a doped glass 
 *  	(includes dopant segregation & interface transport)
 *
 * Level 3: 
 *  - Oxide growth and dopant redistribution 
 *  	including oxidation-ehanced diffusion.
 * 
 * Author: Henry Hellbusch
 * Date: 10/5/2013
 *
 * Team Members: Nate Walsh, Will Abisalih, Nicholas Edwards
 *
 * number of beers consumed while programming this:
 * 	Henry : 3
 */


#include <iostream>
#include <string>
#include <sstream>
#include "BigIntegerLibrary.hh"
#include "Wafer.h"
#include "PeriodicElementFactory.h"
#include "matplotpp.h"

using namespace std;

class MP :public MatPlot
{ 
	void DISPLAY()
	{
    	vector<double> x(100),y(100);    
    	for(int i=0;i<100;++i)
    	{
			x[i]=0.1*i;
			y[i]=sin(x[i]);
    	}
    	plot(x,y);
    }
}mp;

void display(){ mp.display(); }
void reshape(int w,int h){ mp.reshape(w,h); }

string makeBigString(int base, int exponent)
{
	stringstream ss;
	ss << base;
	for (int i = 0; i < exponent; i++)
	{
		ss << 0;
	}
	return ss.str();
}

int main(int argc, char *argv[]) 
{
	cout << "launching ritprem" << endl;
	PeriodicElementFactory periodicElemFactory;
	PeriodicElement boron = periodicElemFactory.getElement("B");
	BigUnsigned conc = stringToBigUnsigned(makeBigString(2, 15));

	Wafer wafer(6.0, 0.01, Concentration(boron, conc)); 
	
	glutInit(&argc, argv);
	glutCreateWindow(100,100,400,300);
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutMainLoop();  
	return 0;
}

