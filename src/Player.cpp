#include "Player.h"
#include <iostream>

Player::Player(){

}
Player::Player(string n)
{
    name = n;
    score = 0;
    lives = 3;
}

void Player::incScore(time_t time){
    if(time<=1)     //Quick time
        score += 20;
    else
        score += (20.0 * (1.0/time));
}

void Player::resetScore(){
    score = 0;
}

void Player::decLife(){
    if(lives!=0)
        lives -= 1;
}

void   Player::resetLife(){ lives = 3;}
void   Player::decScore(){score -= 5;}
double Player::getScore(){return score;}
short int Player::getLives(){return lives;}
string Player::getName(){return name;}

Player::~Player()
{

}
