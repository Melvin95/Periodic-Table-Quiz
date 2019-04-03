#include "Question.h"
#include "Element.h"

#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <ostream>

Question::Question(){
    //Nothing is initialized
}

Question::Question(unsigned short int l)
{
    level = l;
    if(l==1)
        levelOne();
    else if(l==2)
        readIn("Questions/Level2.txt");
    else if(l==3)
        readIn("Questions/Level3.txt");
}

void Question::levelOne(){
    //Seed for random number generator, without it, rand() would generate same sequence of atomic numbers every time
    srand(time(0));
    for(unsigned short int i = 0; i<5;i++){
        unsigned short int atomicNum = 1 + rand()%118;     //Get random questions

        //Make sure we haven't already picked this atomicNum
        for(unsigned short int j = 0; j<answerVector.size();j++){
            while(answerVector[j]==atomicNum)
                atomicNum = 1 + rand()%118;     //Find another atomicNum
        }
        Element element(atomicNum);

        questionVector.push_back("What's the atomic number of "+element.getName());
        answerVector.push_back(element.getAtomicNumber());
    }
}

//Converts a string of atomic numbers to a vector of atomic numbers
vector<unsigned short int> convertToInt(string& s){
    vector<unsigned short int> v;
    unsigned short int a;
    stringstream ss(s);
    while(true){
        ss>>a;
        if(!ss)
            break;

        v.push_back(a);
    }
    return v;
}

void Question::readIn(string fileName){
    ifstream file;
    file.open(fileName);

    if(!file.fail()){
        string line;
        int indexOfHash;
        while(getline(file,line)){
            indexOfHash = line.find("#");
            questionVector.push_back(line.substr(0,indexOfHash));
            line = line.substr(indexOfHash+1,line.length());
            vector<unsigned short int> v = convertToInt(line);
            answerMatrix.push_back(v);
        }
    }
    else
        cout << "File not found";

    file.close();
}

string Question::answerToString(unsigned short int& index){
    ostringstream outstr;
    if(level==1)
        outstr << answerVector[index];
    else{
        for(unsigned short int a = 0 ; a<answerMatrix[index].size();a++)
            outstr << answerMatrix[index][a]<<" ";
    }
    return outstr.str();
}

vector<unsigned short int> Question::getAnswer(){
    return answerVector;
}

vector<vector<unsigned short int> > Question::getAnswerMatrix(){
        return answerMatrix;
}
vector<string> Question::getQuestion(){
    return questionVector;
}

void Question::toString(vector<unsigned short int>& v){
    for(unsigned short int i = 0; i<v.size(); i++)
         cout<<v[i]<<" ";
    cout<<endl<<endl;
}

Question::~Question()
{

}
