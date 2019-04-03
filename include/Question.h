#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include<string>
#include<vector>
#include<cstdlib>

using namespace std;

/*
Level One: Level 1 is pretty generic, more efficient just to use the Element Class(check implementation .cpp)
           No text file, generic questions based on randomly generating an atomic number
           20 questions in questionVector corresponding to 20 answers in answerVector
           questionVector[i] = "Neon?"
           answerVector[i] = 10;
           But if we going to use a text file for level 1 too, implementation for level 2 and 3 uses a text file anyway

Level 2&3: Have to read from a text file here
           Question#Answer
           questionVector[i], same as level 1, corresponds to a question
           But question in level 2&3 can have many answers(atomic numbers)
           So need a 2D vector to store answers for these levels
*/

class Question
{
    public:
        Question();
        Question(unsigned short int level);          //Question for a specific level

        vector<string> getQuestion();                 //Returns questionVector
        vector<unsigned short int> getAnswer();      //Returns answerVector, use for level 1 only
        //2D vector, the space between is not optional
        vector<vector<unsigned short int> > getAnswerMatrix(); //Return answerMatrix, use level 2 and 3 only
        void toString(vector<unsigned short int>&);  //Prints out a vector,

        string answerToString(unsigned short int& index);       //Returns answer as a string, any level
        virtual ~Question();

    protected:
    private:
        void readIn(string);                //Generates level 2 and 3 questions and answers from text file
        void levelOne();                    //Generates level one type question(name?) and answers(atomic number)

        vector<string> questionVector;
        vector<unsigned short int> answerVector;          //Answers for questions in level 1
        vector<vector<unsigned short int> > answerMatrix; //Answers for questions in level 2 and 3
        unsigned short int level;
};
#endif // QUESTION_H
