#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    int getLevel();
    void tryToInfectSomeone(int x, int y);
    void addActor(Actor* newone) { m_actor.push_back(newone);}
    void finish_level() { finished_level = true;}
    void gotGoodies(int type);
    void penelopeDead() { m_penelope->setToDead();}
    int actorDead(int num);
    void decCitizen() { num_citizen--;}
    int num_citizen_left() { return num_citizen;}
    string getScoreText();
    bool walkable(int x, int y, int ox, int oy);
    bool objectHere(int x, int y);
    int characterHere(int x, int y, int range, int ID);
    int goodieHere(int x, int y, int range);
    bool flamableHere(int x, int y, int ID);
    int distToCharacter(int x, int y, int ID);
    bool penelopeHere(int x, int y, int range);
    bool overlap(int x, int y, int ax, int ay);
    bool closerToPenelope(int x, int y, int dir);
    int samercPenelope(int x, int y);
    //bool withinRange(int x, int y, int ax, int ay, int range);
    int dist(int x, int y, int ax, int ay);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
private:
    //GameWorld* m_gameworld;
    Penelope* m_penelope;
    vector<Actor*> m_actor;
    int num_citizen;
    bool finished_level;
    string score_text;
};

#endif // STUDENTWORLD_H_
