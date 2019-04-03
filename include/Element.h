#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <cstdlib>

using namespace std;
class Element
{
    public:
        Element(unsigned short int atomicNumber);
        string getName();
        string getSymbol();
        unsigned short int getAtomicNumber();
        float getAtomicMass();

        virtual ~Element();

    protected:

    private:
        void setValues(unsigned short int atomicNumber);     //Initializes values below for a particular element
        unsigned short int atomicNumber;
        string elementName;
        string elementSymbol;
        float atomicMass;
};

#endif // ELEMENT_H
