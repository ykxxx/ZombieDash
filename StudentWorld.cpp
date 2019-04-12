#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    //m_gameworld = this;
    num_citizen = 0;
    finished_level = false;
    m_penelope = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::getLevel() {
    
    Level lev(assetPath());
    string level = to_string(GameWorld::getLevel());
    if (GameWorld::getLevel() < 10) {
        level = '0' + level;
    }
    string levelFile = "level" + level + ".txt";
    //string levelFile = "level04.txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found || level == "99") {
        //cerr << "Cannot find" << levelFile << " data file" << endl;
        return 0;
    }
    else if (result == Level::load_fail_bad_format) {
        //cerr << "Your level was improperly formatted" << endl;
        return -1;
    }
    else if (result == Level::load_success) {
        //cerr << "Successfully loaded level" << endl;
        
        for (int i = 0; i < LEVEL_WIDTH; i++) {
            for (int j = 0; j < LEVEL_HEIGHT; j++) {
                Level::MazeEntry ge = lev.getContentsOf(i, j); // level_x=5, level_y=10
                double x = i * 16.0;
                double y = j * 16.0;
                switch (ge) { // so x=80 and y=160
                    case Level::empty:
                        break;
                    case Level::smart_zombie:
                        m_actor.push_back(new SmartZombie(this, IID_ZOMBIE, x, y));
                        break;
                    case Level::dumb_zombie:
                        m_actor.push_back(new DumbZombie(this, IID_ZOMBIE,  x, y));
                        break;
                    case Level::player:
                        m_penelope = new Penelope(this, IID_PLAYER, x, y);
                        break;
                    case Level::exit:
                        m_actor.push_back(new Exit(this, IID_EXIT, x, y));
                        break;
                    case Level::wall:
                        m_actor.push_back(new Wall(this, IID_WALL, x, y));
                        break;
                    case Level::pit:
                        m_actor.push_back(new Pit(this, IID_PIT, x, y));
                        break;
                    case Level::citizen:
                        m_actor.push_back(new Citizen(this, IID_CITIZEN,  x, y));
                        num_citizen++;
                        break;
                    case Level::vaccine_goodie:
                        m_actor.push_back(new Vaccine(this, IID_VACCINE_GOODIE, x, y));
                        break;
                    case Level::gas_can_goodie:
                        m_actor.push_back(new GasCan(this, IID_GAS_CAN_GOODIE,  x, y));
                        break;
                    case Level::landmine_goodie:
                        m_actor.push_back(new LandmineGoodies(this, IID_LANDMINE_GOODIE,  x, y));
                        break;
                }
            }
        }
    }
    return 1;
}

void StudentWorld::tryToInfectSomeone(int x, int y) {
    int num = characterHere(x, y, 0, 1);
    if (num != -1) {
        m_actor[num]->infected();
    }
    else if (penelopeHere(x, y, 0)) {
        m_penelope->infected();
    }
}

void StudentWorld::gotGoodies(int type) {
    switch (type) {
        case 1: // gascan
            m_penelope->addFlamethrower(5);
            break;
        case 2: // vaccaine
            m_penelope->addVaccines(1);
            break;
        case 3: // landmine
            m_penelope->addLandmies(2);
            break;
    }
}

int StudentWorld::actorDead(int num) {
    m_actor[num]->setToDead();
    return m_actor[num]->getScore();
}

bool StudentWorld::walkable(int x, int y, int ox, int oy) {
    for (int i = 0; i < m_actor.size(); i++) {
        int ax = m_actor[i]->getX();
        int ay = m_actor[i]->getY();
        if (ox == ax && oy == ay) {
            continue;
        }
        if (!m_actor[i]->canWalkHere()) {
            if (overlap(x, y, ax, ay)) {
                return false;
            }
        }
    }
    if (!(ox == m_penelope->getX() && oy == m_penelope->getY())) {
        if (overlap(x, y, m_penelope->getX(), m_penelope->getY())) {
            return false;
        }
    }
    return true;
}

int StudentWorld::characterHere(int x, int y, int range, int ID) {
    for (int i = 0; i < m_actor.size(); i++) {
        int ax = m_actor[i]->getX();
        int ay = m_actor[i]->getY();
        switch (ID) {
            case 1:
                if (m_actor[i]->isCharacter() == 1 && m_actor[i]->isAlive()) {
                    switch (range) {
                        case 0:
                            if (overlap(x, y, ax, ay)) {
                                return i;
                            }
                            break;
                        default:
                            if (dist(x, y, ax, ay) <= range) {
                                return i;
                            }
                            break;
                    }
                }
                break;
            case 2:
                if (m_actor[i]->isCharacter() == 2 && m_actor[i]->isAlive()) {
                    switch (range) {
                        case 0:
                            if (overlap(x, y, ax, ay)) {
                                return i;
                            }
                            break;
                        default:
                            if (dist(x, y, ax, ay) <= range) {
                                return i;
                            }
                            break;
                    }
                }
                break;
        }
    }
    return -1;
}

int StudentWorld::goodieHere(int x, int y, int range) {
    for (int i = 0; i < m_actor.size(); i++) {
        int ax = m_actor[i]->getX();
        int ay = m_actor[i]->getY();
        if (m_actor[i]->isGoodies() != -1) {
            switch (range) {
                case 0:
                    if (overlap(x, y, ax, ay)) {
                        return i;
                    }
                    break;
                default:
                    if (dist(x, y, ax, ay) <= range) {
                        return i;
                    }
                    break;
            }
        }
    }
    return -1;
}

bool StudentWorld::flamableHere(int x, int y, int ID) {
    bool here = true;
    for (int i = 0; i < m_actor.size(); i++) {
        int ax = m_actor[i]->getX();
        int ay = m_actor[i]->getY();
            switch (ID) {
                case 0:
                    here = false;
                    if (m_actor[i]->isFlamable() == 0) {
                        if (overlap(x, y, ax, ay)) {
                            return true;
                        }
                    }
                    break;
                case 1:
                    if (m_actor[i]->isFlamable() == -1 || m_actor[i]->isFlamable() == 0) {
                        if (overlap(x, y, ax, ay)) {
                            return false;
                        }
                    }
                    break;
            }
        }
    return here;
}

bool StudentWorld::penelopeHere(int x, int y, int range) {
    int ax = m_penelope->getX();
    int ay = m_penelope->getY();
    switch (range) {
        case 0:
            if (overlap(x, y, ax, ay)) {
                return true;
            }
            break;
        default:
            if (dist(x, y, ax, ay) <= range) {
                return true;
            }
            break;
    }
    return false;
}

bool StudentWorld::objectHere(int x, int y) {
    for (int i = 0; i < m_actor.size(); i++) {
        if (overlap(x, y, m_actor[i]->getX(), m_actor[i]->getY())) {
            return false;
        }
    }
    if (penelopeHere(x, y, 0)) {
        return false;
    }
    return true;
}

bool StudentWorld::overlap(int x, int y, int ax, int ay) {
    if (ax >= x - SPRITE_WIDTH + 1 && ax <= x + SPRITE_WIDTH - 1) {
        if (ay >= y && y + SPRITE_HEIGHT - 1 > ay) {
            return true;
        }
        else if (ay <= y && ay + SPRITE_HEIGHT - 1 > y) {
            return true;
        }
    }
    else if (ay >= y - SPRITE_HEIGHT + 1 && ay <= y + SPRITE_HEIGHT - 1) {
        if (ax >= x && x + SPRITE_WIDTH - 1 > ax) {
            return true;
        }
        else if (ax <= x && ax + SPRITE_WIDTH - 1 > x) {
            return true;
        }
    }
    return false;
}

int StudentWorld::dist(int x, int y, int ax, int ay) {
    return sqrt((x - ax) * (x - ax) + (y - ay) * (y - ay));
}

int StudentWorld::distToCharacter(int x, int y, int ID) {
    int distant = 1000;
    switch (ID) {
        case 0:
            distant = dist(x, y, m_penelope->getX(), m_penelope->getY());
            break;
        case 2:
            for (int i = 0; i < m_actor.size(); i++) {
                if (m_actor[i]->isCharacter() == 2) {
                    int new_dist = dist(x, y, m_actor[i]->getX(), m_actor[i]->getY());
                    distant = distant < new_dist ? distant : new_dist;
                }
            }
            break;
    }
    return distant;
}

int StudentWorld::samercPenelope(int x, int y) {
    int ax = m_penelope->getX();
    int ay = m_penelope->getY();
    if (y == ay) {
        if (x < ax) {
            return GraphObject::right;
        }
        else if (x > ax) {
            return GraphObject::left;
        }
    }
    else if (x == ax) {
        if (y < ay) {
            return GraphObject::up;
        }
        else if (y > ay) {
            return GraphObject::down;
        }
    }
    return -1;
}

bool StudentWorld::closerToPenelope(int x, int y, int dir) {
    int ax = m_penelope->getX();
    int ay = m_penelope->getY();
    switch (dir) {
        case GraphObject::up:
            if (ax < x) {
                return true;
            }
            break;
        case GraphObject::left:
            if (ay < y) {
                return true;
            }
            break;
    }
    return false;
}
// Score: 004500  Level: 27  Lives: 3  Vaccines: 2  Flames: 16  Mines: 1  Infected: 0
string StudentWorld::getScoreText() {
    ostringstream score;
    score << "Score: ";
    score.fill('0');
    score << setw(6) << GameWorld::getScore();
    score << "  Level: " << GameWorld::getLevel();
    score << "  Lives: " << GameWorld::getLives();
    score << "  Vaccines: " << m_penelope->getVaccines();
    score << "  Flames: " << m_penelope->getFlamethrower();
    score << "  Mines: " << m_penelope->getLandmies();
    score << "  Infected: " << m_penelope->getInfectionCount();
    
    string test = score.str();
    return test;
}

//Score: 004500 Level: 27 Lives: 3 Vaccines: 2 Flames: 16 Mines: 1 Infected: 0
int StudentWorld::init()
{
    finished_level = false;
    int status = getLevel();
    
    if (status == 0) {
        return GWSTATUS_PLAYER_WON;
    }
    else if (status == -1) {
        return GWSTATUS_LEVEL_ERROR;
    }
    else {
        string test = getScoreText();
        GameWorld::setGameStatText(test);
        return GWSTATUS_CONTINUE_GAME;
    }
}

int StudentWorld::move()
{
    m_penelope->doSomething();
    for (int i = 0; i < m_actor.size(); i++) {
        if (m_actor[i]->isAlive()) {
            m_actor[i]->doSomething();
        }
        if (!m_actor[i]->isAlive()) {
            m_actor[i]->dropVaccine();
            delete m_actor[i];
            m_actor.erase(m_actor.begin() + i);
        }
    }
    if (!m_penelope->isAlive()) {
        GameWorld::decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    else if (finished_level == true) {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // remove all dead actor in the current tick, and remove from actor vector
    string test = getScoreText();
    GameWorld::setGameStatText(test);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (!m_actor.empty()) {
        delete m_actor.back(); // delete dynamically allocated object
        m_actor.pop_back(); // pop the memory
    }
    delete m_penelope;
}
