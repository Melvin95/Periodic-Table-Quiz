#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;
class Player
{
    public:
        Player();
        Player(string name);

        string getName();
        double getScore();
        short int getLives();

        void decLife();
        void resetLife();
        void incScore(time_t);
        void decScore();
        void resetScore();

        virtual ~Player();

    protected:

    private:
        void addPlayer();   //Only if player is a new player

        string name;
        double score;      //Score for single instance of a game, not the accumalated score from past plays(in scoreboard)
        short int lives;
};

#endif // PLAYER_H
