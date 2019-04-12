#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>

// isCharacter = 0 for penelope, 1 for citizen, 2 for zombie, -1 for others
// isGoodies = 1 for gascan, 2 for vaccine, 3 for landmine goodies, -1 for others
// isFlamable = f-1 for exit and wall, 1 for flamable and 0 for flame

using namespace std;

class StudentWorld;
class Goodies;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : GraphObject(imageID, startX, startY, dir, depth, size), m_sw(sw) {}
    StudentWorld* getWorld();
    virtual void setToDead() { return;}
    virtual bool isAlive() { return true;}
    virtual void doSomething() = 0;
    virtual void dropVaccine() { return;}
    virtual bool canWalkHere() { return true;}
    virtual int isFlamable() { return 1;}
    virtual int isCharacter() { return -1;}
    virtual int isGoodies() { return -1;}
    virtual int getScore() { return 0;}
    virtual void infected() { return;};
private:
    StudentWorld* m_sw;
};

class Penelope : public Actor {
public:
    Penelope(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size), m_vaccines(0), m_landmines(0), m_flamethrower(0), infection_count(0), is_infected(false), is_alive(true)
    {}
    int getVaccines() { return m_vaccines;}
    int getLandmies() { return m_landmines;}
    int getFlamethrower() { return m_flamethrower;}
    int addVaccines(int num) { return m_vaccines += num;}
    int addLandmies(int num) { return m_landmines += num;}
    int addFlamethrower(int num) { return m_flamethrower += num;}
    int getInfectionCount() { return infection_count;}
    bool isInfected() { return is_infected;}
    virtual bool isAlive() { return is_alive;}
    virtual void doSomething();
    virtual bool canWalkHere() { return false;}
    virtual int isCharacter() {return 0;}
    virtual void setToDead() { is_alive = false;}
    virtual void infected() {is_infected = true;}
private:
    int m_vaccines = 0;
    int m_landmines = 0;
    int m_flamethrower = 0;
    int infection_count = 0;
    bool is_infected = false;
    bool is_alive = true;
    //vector<Goodies*> m_inventory;
};

class Wall : public Actor {
public:
    Wall(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual bool canWalkHere() { return false;}
    virtual int isFlamable() { return -1;}
    virtual void doSomething() { return;}
};

class Exit : public Actor {
public:
    Exit(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size) {}
    virtual int isFlamable() { return -1;}
    virtual void doSomething();
};

class Citizen : public Actor {
public:
    Citizen(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual bool canWalkHere() { return false;}
    virtual int isCharacter() { return 1;}
    virtual void doSomething();
    virtual void setToDead() { is_alive = false;}
    virtual void infected() {is_infected = true;}
    virtual bool isAlive() { return is_alive;}
    virtual int getScore() { return -1000;}
private:
    int infection_count = 0;
    bool is_infected = false;
    bool is_alive = true;
    bool is_paralyzed = false;
};

class Zombie : public Actor {
public:
    Zombie(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual void setToDead() { is_alive = false;}
    virtual bool canWalkHere() { return false;}
    virtual int isCharacter() { return 2;}
    virtual bool isAlive() { return is_alive;}
    virtual int getScore() { return 1000;}
    virtual void doSomething();
private:
    int movement_plan = 0;
    bool is_alive = true;
    bool is_paralyzed = false;
};

class SmartZombie: public Zombie {
public:
    SmartZombie(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Zombie(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual int getScore() { return 2000;}
    virtual void doSomething();
private:
};

class DumbZombie : public Zombie {
public:
    DumbZombie(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Zombie(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual void dropVaccine();
    virtual void doSomething();
private:
};

class Vomit : public Actor {
public:
    Vomit(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual void doSomething();
    virtual void setToDead() { is_alive = false;}
    virtual bool isAlive() { return is_alive;}
private:
    int alive_tick = 0;
    bool is_alive = true;
};

class Goodies : public Actor {
public:
    Goodies(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual bool isAlive() { return is_alive;}
    virtual void setToDead() { is_alive = false;}
    virtual void doSomething() = 0;
private:
    bool is_alive = true;
};

class GasCan: public Goodies {
public:
    GasCan(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Goodies(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual int isGoodies() { return 1;}
    virtual void doSomething();
};

class Vaccine : public Goodies {
public:
    Vaccine(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Goodies(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual int isGoodies() { return 2;}
    virtual void doSomething();
};

class LandmineGoodies : public Goodies {
public:
    LandmineGoodies(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Goodies(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual int isGoodies() { return 3;}
    virtual void doSomething();
};

class Pit : public Actor {
public:
    Pit(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual void doSomething();
};

class Flame : public Actor {
public:
    Flame(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth = 0, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual int isFlamable() { return 0;}
    virtual bool isAlive() { return is_alive;}
    virtual void doSomething();
private:
    int m_tick = 0;
    bool is_alive = true;
};

class Landmine : public Actor {
public:
    Landmine(StudentWorld* sw, int imageID, double startX, double startY, Direction dir = 0, int depth = 1, double size = 1.0)
    : Actor(sw, imageID, startX, startY, dir, depth, size)
    {}
    virtual bool isAlive() { return is_alive;}
    virtual void setToDead() { is_alive = false;}
    virtual void doSomething();
private:
    void explode(int x, int y);
    int m_saftytick = 30;
    bool inactive = true;
    bool is_alive = true;
};



#endif // ACTOR_H_
