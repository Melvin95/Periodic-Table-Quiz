#include "Element.h"

#include <iostream>
#include <fstream>
#include <regex>
Element::Element(unsigned short int a)
{
    atomicNumber = a;
    setValues(a);
}

void Element::setValues(unsigned short int a){
    ifstream file;
    file.open("PeriodicTableData.txt");

    if(!file.fail()){
        string line;
        for(unsigned short int i = 1; i<a;i++)
            getline(file,line);
        file>>atomicMass;
        file>>elementName;
        file>>elementSymbol;
    }
    else
        cout<<"Error opening file";

    file.close();
}

string Element::getName(){return elementName;}
string Element::getSymbol(){return elementSymbol;}
//string Element::getGroup(){return elementGroup;}
unsigned short int Element::getAtomicNumber(){return atomicNumber;}
float Element::getAtomicMass(){return atomicMass;}

Element::~Element()
{
    //dtor
}
