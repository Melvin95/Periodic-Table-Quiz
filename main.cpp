#include "Player.h"
#include "Question.h"

#include <ctime>
#include <fstream>

using namespace std;
/*SINGLE PLAYER. Console periodic table game, player answers in atomic number

  Split the implementation of level 1 completely from level 2/3 because...
  Level 1: Each question has exactly one answer
           Q: Hydrogen? A = 1.
           Q: Helium?   A = 2.
  Level 2/3: Each question can have more than one answer
           Q: Noble Gas? A = 2 , 10 , 18 , 36 ...
           Q: Water?     A = 1 , 8
  This  results in a different data structure(1D vector vs 2D vector) being used to store the answers from level 1 and leve2/3

  A small difference between level 2 & 3
        Level 2: one answer from player, answer must just be contained in the answers[ranIndex] vector
        Level 3: multiple answers from player, these answers = answers[randIndex] vector

 NOTE: Still inefficient, should be passing using constant reference whenever not changing a variable, just referencing
       vector.push_back() inefficient also, copies entire vector when adding a new element
       could be avoided if we knew max size of a vector before hand
*/
void saveScore(Player,unsigned short int);      //Writes player and score to text file

void playLevelOne(short int);
void playLevelTwo(short int);
void playLevelThree(short int);

void askQuestion();

//---------------------------------------GLOBAL VARIABLES---------------------------------------------//
string name;
short int level;

Player* player;
Question* q;

vector<string> questions;
vector<unsigned short int> answers1;    //Level 1 answers
vector<vector<unsigned short int> > answers; //Level 2/3 answers
vector<bool> chosen;    //Helper vector in level 2/3, to make sure questions aren't repeated


short int response;           //For player input
vector<short int> responses; //Stores multiple player input(used in level 3)
short int num;              //Number of unique questions per round
short int ranIndex;         //Randomizes index of questionVector for level 2/3
bool correct;

//Time variable to track time taken for player to answer question
time_t t1;      //As soon as question is rendered
time_t t2;      //As soon as responses thats needed for a question are given


//--------------------------------------------------------------------------------------------------//

int main()
{
    cout << "Enter player name: ";
    cin  >> name;
    cout << "What level?";
    cin >> level;

    player = new Player(name);
    q = new Question(level);

    do{
        srand(time(0));     //Makes sure 1st question(ranIndex initialization) for level 1/2 aren't same all the time
        if(level==1){
            //Initialize level 1 variables
            questions = q->getQuestion();
            answers1  = q->getAnswer();
            //askQuestion() sort of a dummy function in playing the level, that just renders questions
            //Each time askQuestion() is called, it sends player's response as a parameter to playLevel(response)
            askQuestion();  //Called multiple times in playLevelOne(), only stop asking when round round is over
            //Round is over(all questions asked), save scores
            cout<<"Level 1 completed"<<endl;
            cout<<"Score for this round: "<<player->getScore()<<endl;
            saveScore(*player,1);  //Writes players score to a text file
            player->resetScore(); //Round is finished, reset player's score, don't want to carry that score into other levels/rounds
        }
        else if(level==2){
            questions = q->getQuestion();
            answers = q->getAnswerMatrix();
            num = 3;
            ranIndex = rand()%questions.size(); //Get a random question, this is the 1st time, so no need to check if it's been asked
            chosen.resize(questions.size(),false);

            askQuestion();

            chosen.resize(0);   //For next set of questions(same or different level) like deallocating old data
            cout<<"Level 2 completed"<<endl;
            cout<<"Score for this round: "<<player->getScore()<<endl;
            saveScore(*player,2);  //Writes players score to a text file
            player->resetScore(); //Round is finished, reset player's score, don't want to carry that score into other levels/rounds
        }
        else if(level==3){
            questions = q->getQuestion();
            answers = q->getAnswerMatrix();
            num = 3;
            ranIndex = rand()%questions.size();
            chosen.resize(questions.size(),false);  //Initialize/populate chosen vector

            t1 = time(0);       //Record start time now, this only for 1st question, t1 changes in playLevelOne() for other questions
            askQuestion();      //Ask 1st question

            chosen.resize(0);
            cout<<"Level 3 completed"<<endl;
            cout<<"Score for this round: "<<player->getScore()<<endl;
            saveScore(*player,3);  //Writes players score to a text file
            player->resetScore(); //Round is finished, reset player's score, don't want to carry that score into other levels/rounds
        }

        //Maybe player wants to play another level after done with level chosen before
        cout<<endl<<"What level?"<<endl;
        cin >> level;
        delete q;
        q = new Question(level);
    }
    while(level!=-1);
    delete player;

    return 0;
}

void askQuestion(){
    if(level==1){
        cout << questions[questions.size()-1]<<endl;
        t1 = time(0);
        cin >> response;
        t2 = time(0);
        playLevelOne(response);
    }
    else if(level==2){
        cout << questions[ranIndex]<<endl;
        t1 = time(0);
        cin >> response;
        t2 = time(0);
        playLevelTwo(response);
    }
    else if(level==3){
        cout<<questions[ranIndex]<<endl;
        cin >> response;
        responses.push_back(response);
        playLevelThree(response);
    }
}

void playLevelOne(short int response){
        //If player's answer incorrect, decrement life and score
        //Handle two cases, if player still has lives or doesn't
        if(response!=answers1[answers1.size()-1]){
            player->decLife();
            player->decScore();
            //If player has 0 lives, can't attempt same question again so...
            //Give them the correct answer, reset lives for next question and
            //Remove that pair of question and answer asked already
            if(player->getLives()==0){
                cout<<"WRONG. Out of attempts. Correct answer was: "<<answers1[answers1.size()-1]<<endl<<endl;
                player->resetLife();
                questions.pop_back();
                answers1.pop_back();
            }
            else{//Player still has attemtps left on the question, ask it again
                cout<<"WRONG. Attempts left: "<<player->getLives()<<endl;
                askQuestion();  //Asks the same question again
            }
        }
        //Else player got answer correct, move on to next question then
        else{
            player->incScore(t2-t1);
            player->resetLife();    //Reset life for next question
            cout << "CORRECT"<<endl<<endl;
            //Remove question and answer pair(asked already)
            questions.pop_back();
            answers1.pop_back();
        }
        //If questions left to ask
        if(!questions.empty())
            askQuestion();  //Asks a new question
}

void playLevelTwo(short int response){
    //Validate
    //Simply checking if response is in answer[randIndex] vector(like list.contains(response))
    correct = false;
    for(unsigned short int i = 0; i< answers[ranIndex].size(); i++){
            if(response==answers[ranIndex][i]){
                correct = true;
                break;      //As soon as match is found, break, no need to iterate through entire vector
            }
    }

    if(correct==true){
        cout<<"CORRECT"<<endl<<endl;
        player->incScore(t2-t1);
        player->resetLife();
        chosen[ranIndex] = true;  //Indicate this question has been asked so can ask another question
        num--;
    }
    else{
        player->decLife();
        player->decScore();
        if(player->getLives()==0){
            cout<<"WRONG. Out of attempts. Correct answers could have been: ";
            q->toString(answers[ranIndex]);
            cout<<endl<<endl;
            player->resetLife();
            chosen[ranIndex] = true; //Move onto next question, this question asked enough
            num--;
        }
        else{
            cout << "WRONG. Attempts left: "<<player->getLives()<<endl;
            askQuestion();  //Ask the same question
        }
    }
    //We only reach this point if player is correct or out of attempts
    //Thus need to ask next question if number of questions per round isn't satisfied
    srand(time(0));     //Makes sure sequence of random numbers is not the same every runtime
    if(num!=0){//If number of questions not satisfied
        ranIndex = rand()%questions.size(); //Get a random question
        while(chosen[ranIndex]==true)   //If it's been asked already, randomize till one that hasn't been asked is found
           ranIndex = rand()%questions.size();
        askQuestion(); //Ask the question with the new randIndex
    }
    //Else returning to main(), end of round...
}

void playLevelThree(short int response){
    //If we haven't got enough responses from the player
    if(responses.size()!=answers[ranIndex].size()){

        askQuestion();  //Ask for another response,asking same question
    }

    /*Else, validate responses vector
      Changed this slightly from the previous version because I messed up the logic for level 3
      The player could've answered "11 11" or "17 17" to "what elements make up salt" and got it correct
      Obviously, it should be evaluated as incorrect.
      Fix: Make sure for every answers[ranIndex] element, there's a corresponding response in responses vector
      Basically switched the for-loops
    */
    else{
        t2 = time(0);           //At this point, all responses recorded, EndTime

        correct = true;         //Assume answers player entered are correct
        bool foundMatch = false;//Whether there's a match in responses for a specific element in answers[ranIndex]

        for(unsigned short int a =0; a<answers[ranIndex].size();a++){ //Loop through answers
            for(unsigned short int r = 0; r<responses.size();r++){    //Loop through responses
                if(answers[ranIndex][a]==responses[r]){
                    foundMatch = true;  //if there's a match in responses vector for an answer
                    break;             //then break from inner loop
                }
            }
            if(foundMatch==false){          //If no match in responses(therefore particular response is wrong)
                correct = false;            //then players entire responses are incorrect
                break;                      //break from outer-loop
            }
            else
                foundMatch = false;         //else need to check other answers
        }

        //At this point, current responses been evaluated, clear it to allow new responses
        responses.clear();

        if(correct==true){
            player->incScore(t2-t1);
            player->resetLife();
            cout<<"CORRECT"<<endl<<endl;
            chosen[ranIndex] = true;   //Player got question right, no need to ask question again
            num--;
        }
        else{
            player->decLife();
            player->decScore();
            if(player->getLives()==0){
                cout<<"WRONG.Out of attempts. Correct answers were: ";
                q->toString(answers[ranIndex]);
                cout<<endl<<endl;
                player->resetLife();
                chosen[ranIndex] = true; //Player got question wrong on all attempts, question fully asked
                num--;
            }
            else{
                cout<<"WRONG. Attempts left: "<<player->getLives()<<endl;
                t1 = time(0);   //Start time for answering same question again is different
                askQuestion(); //Player still has attempts left, ask same question again
            }
        }
  }
    //Asking a new question:
    srand(time(0));
    if(num!=0){//If number of questions not satisfied
        ranIndex = rand()%questions.size(); //Get a random question
        while(chosen[ranIndex]==true)   //If it's been asked already, randomize till one that hasn't been asked is found
            ranIndex = rand()%questions.size();
        t1 = time(0);   //Start time
        askQuestion(); //Ask the question with the new ranIndex
    }
}


void saveScore(Player player,unsigned short int level){
    string fileName;
    if(level==1)
        fileName = "ScoreBoard/LevelOneScoreboard.txt";
    else if(level==2)
        fileName = "ScoreBoard/LevelTwoScoreboard.txt";
    else if(level==3)
        fileName = "ScoreBoard/LevelThreeScoreboard.txt";

    ofstream file;
    file.open(fileName,fstream::app);

    if(!file.fail()){
        file<<player.getName()<<"#"<<player.getScore()<<"\n";
    }
    else
        cout<<"File not found";
    file.close();
}
