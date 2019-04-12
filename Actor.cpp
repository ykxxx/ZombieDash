#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

void Actor::doSomething() {
//    If the player is in front of me and close by, then Vomit in the direction of the player
//    Else if I still want to continue moving in the current direction
//        Move one pixel in my current direction
//        Decrement the number of remaining ticks to move in this direction
//        Else if I want to choose a new direction
//            Pick a new direction to move, and pick how many ticks to move in that direction.
}

StudentWorld* Actor::getWorld() {
    return m_sw;
}

void Penelope::doSomething() {
    int ch;
    int x = getX();
    int y = getY();
    
    if (!is_alive) {
        return;
    }
    else if (is_infected) {
        infection_count++;
        if (infection_count == 500) {
            is_alive = false;
            getWorld()->GameWorld::playSound(SOUND_PLAYER_DIE);
            return;
        }
    }
    
    if (getWorld()->getKey(ch)) {
        // user hit a key during this tick!
        switch (ch) {
            case KEY_PRESS_UP:
                setDirection(up);
                if (getWorld()->walkable(getX(), getY() + 4, getX(), getY())) {
                    moveTo(getX(), getY() + 4);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (getWorld()->walkable(getX(), getY() - 4, getX(), getY())) {
                    moveTo(getX(), getY() - 4);
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (getWorld()->walkable(getX() - 4, getY(), getX(), getY())) {
                    moveTo(getX() - 4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                 setDirection(right);
                if (getWorld()->walkable(getX() + 4, getY(), getX(), getY())) {
                    moveTo(getX() + 4, getY());
                }
                break;
            case KEY_PRESS_SPACE: // ADD FLAME
                if (m_flamethrower > 0) {
                    m_flamethrower--;
                    getWorld()->GameWorld::playSound(SOUND_PLAYER_FIRE);
                    int dir = getDirection();
                    for (int i = 1; i <= 3; i++) {
                        switch (dir) {
                            case up:
                                y = y + SPRITE_HEIGHT;
                                break;
                            case down:
                                y = y - SPRITE_HEIGHT;
                                break;
                            case left:
                                x = x - SPRITE_WIDTH;
                                break;
                            case right:
                                x = x + SPRITE_WIDTH;
                                break;
                        }
                        if (!getWorld()->flamableHere(x, y, 1)) {
                            continue;
                        }
                        getWorld()->addActor(new Flame(getWorld(), IID_FLAME, x, y, dir));
                    }
                }
                break;
            case KEY_PRESS_TAB: // add lanndmine
                if (m_landmines > 0) {
                    m_landmines--;
                    getWorld()->addActor(new Landmine(getWorld(), IID_LANDMINE, x, y));
                }
                break;
            case KEY_PRESS_ENTER: // inject vaccine
                if (m_vaccines > 0) {
                    is_infected = false;
                    infection_count = 0;
                    m_vaccines--;
                }
                break;
        }
    }
}

void Exit::doSomething() {
    // check if citizen if here
    int id = getWorld()->characterHere(getX(), getY(), 0, 1);
    if (id != -1) {
        getWorld()->GameWorld::increaseScore(500);
        getWorld()->actorDead(id);
        getWorld()->GameWorld::playSound(SOUND_CITIZEN_SAVED);
        getWorld()->decCitizen();
    }
    else if (getWorld()->penelopeHere(getX(), getY(), 0) && getWorld()->num_citizen_left() == 0) {
        getWorld()->GameWorld::advanceToNextLevel();
        getWorld()->finish_level();
        getWorld()->playSound(SOUND_LEVEL_FINISHED);
    }
}

void Citizen::doSomething() {
    int x = getX();
    int y = getY();
    if (!is_alive) {
        getWorld()->playSound(SOUND_CITIZEN_DIE);
        return;
    }
    else if (is_infected) {
        if (infection_count == 0) {
            getWorld()->playSound(SOUND_CITIZEN_INFECTED);
        }
        infection_count++;
        if (infection_count >= 500) {
            is_alive = false;
            getWorld()->GameWorld::playSound(SOUND_ZOMBIE_BORN);
            getWorld()->increaseScore(-1000);
            getWorld()->decCitizen();
            
            int random = randInt(1, 10);
            if (random <= 7) {
                getWorld()->addActor(new DumbZombie(getWorld(), 1, x, y));
            }
            else {
                getWorld()->addActor(new SmartZombie(getWorld(), 1, x, y));
            }
            return;
        }
    }
    if (is_paralyzed) {
        is_paralyzed = false;
        return;
    }
    is_paralyzed = true;
    //calculate dis_p to penelope
    int dist_p = getWorld()->distToCharacter(x, y, 0);
    int dist_z = getWorld()->distToCharacter(x, y, 2);
    if (dist_p < dist_z && dist_p <= 80) {
        // on the same row/col
        int dir = getWorld()->samercPenelope(x, y);
        switch (dir) {
            case up:
                y = y + 2;
                if (getWorld()->walkable(x, y, getX(), getY())) {
                    setDirection(dir);
                    moveTo(x, y);
                    return;
                }
                break;
            case down:
                y = y - 2;
                if (getWorld()->walkable(x, y, getX(), getY())) {
                    setDirection(dir);
                    moveTo(x, y);
                    return;
                }
                break;
            case left:
                x = x - 2;
                if (getWorld()->walkable(x, y, getX(), getY())) {
                    setDirection(dir);
                    moveTo(x, y);
                    return;
                }
                break;
            case right:
                x = x + 2;
                if (getWorld()->walkable(x, y, getX(), getY())) {
                    setDirection(dir);
                    moveTo(x, y);
                    return;
                }
                break;
            case -1:
                int dir1 = down; // down
                int dir2 = right;  // right
                if (getWorld()->closerToPenelope(x, y, up)) {
                    dir1 = up; // up
                }
                if (getWorld()->closerToPenelope(x, y, left)) {
                    dir2 = left; // left
                }
                int rand = randInt(1, 2);
                switch (rand) {
                    case 1: // dir1
                        if (dir1 == up) {
                            y = y + 2;
                        }
                        else {
                            y = y - 2;
                        }
                        if (getWorld()->walkable(x, y, getX(), getY())) {
                            setDirection(dir1);
                            moveTo(x, y);
                            return;
                        }
                        else {
                            y = getY();
                            if (dir2 == left) {
                                x = x - 2;
                            }
                            else {
                                x = x + 2;
                            }
                            if (getWorld()->walkable(x, y, getX(), getY())) {
                                setDirection(dir2);
                                moveTo(x, y);
                                return;
                            }
                        }
                        break;
                    case 2: // dir2
                        if (dir2 == left) {
                            x = x - 2;
                        }
                        else {
                            x = x + 2;
                        }
                        if (getWorld()->walkable(x, y, getX(), getY())) {
                            setDirection(dir2);
                            moveTo(x, y);
                            return;
                        }
                        else {
                            x = getX();
                            if (dir1 == up) {
                                y = y + 2;
                            }
                            else {
                                y = y - 2;
                            }
                            if (getWorld()->walkable(x, y, getX(), getY())) {
                                setDirection(dir1);
                                moveTo(x, y);
                                return;
                            }
                        }
                        break;
                }
                break;
        }
    }
    // calculate distance from zombie
    x = getX();
    y = getY();
    if (getWorld()->distToCharacter(getX(), getY(), 2) <= 80) {
        int dist = 0;
        int newdir = -1;
        if (getWorld()->walkable(getX(), getY() + 2, getX(), getY())) {
            int newdist = getWorld()->distToCharacter(getX(), getY() + 2, 2);
            if (newdist > dist) {
                newdir = up;
                dist = newdist;
                y = y + 2;
            }
        }
        if (getWorld()->walkable(getX(), getY() - 2, getX(), getY())) {
            int newdist = getWorld()->distToCharacter(getX(), getY() - 2, 2);
            if (newdist > dist) {
                newdir = down;
                dist = newdist;
                y = y - 2;
            }
        }
        if (getWorld()->walkable(getX() + 2, getY(), getX(), getY())) {
            int newdist = getWorld()->distToCharacter(getX() + 2, getY() , 2);
            if (newdist > dist) {
                newdir = right;
                dist = newdist;
                x = x + 2;
            }
        }
        if (getWorld()->walkable(getX() - 2, getY(), getX(), getY())) {
            int newdist = getWorld()->distToCharacter(getX() - 2, getY() , 2);
            if (newdist > dist) {
                newdir = left;
                dist = newdist;
                x = x - 2;
            }
        }
        if (dist == 0) {
            return;
        }
        else {
            setDirection(newdir);
            moveTo(x, y);
            return;
        }
    }
    return;
}

void Zombie::doSomething() {
    int dir = getDirection();
    
    if (is_paralyzed) {
        is_paralyzed = false;
        return;
    }
    // check to see if vomit
    switch (dir) {
        case 0:
            if (getWorld()->characterHere(getX() + SPRITE_WIDTH, getY(), 10, 1) != -1 || getWorld()->penelopeHere(getX() + SPRITE_WIDTH, getY(), 10)) {
                if (randInt(1, 3) == 1) {
                    getWorld()->addActor(new Vomit(getWorld(), 4, getX() + SPRITE_WIDTH, getY()));
                    getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                    return;
                }
            }
            break;
        case 90: //up
            if (getWorld()->characterHere(getX(), getY() + SPRITE_HEIGHT, 10, 1) != -1 || getWorld()->penelopeHere(getX(), getY() + SPRITE_HEIGHT, 10)) {
                if (randInt(1, 3) == 1) {
                    getWorld()->addActor(new Vomit(getWorld(), 4, getX(), getY() + SPRITE_HEIGHT));
                    getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                    return;
                }
            }
            break;
        case 180: //left
            if (getWorld()->characterHere(getX() - SPRITE_WIDTH, getY(), 10, 1) != -1 || getWorld()->penelopeHere(getX() - SPRITE_WIDTH, getY(), 10)) {
                if (randInt(1, 3) == 1) {
                    getWorld()->addActor(new Vomit(getWorld(), 4, getX() - SPRITE_WIDTH, getY()));
                    getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                    return;
                }
            }
            break;
        case 270: // down
            if (getWorld()->characterHere(getX(), getY() - SPRITE_HEIGHT, 10, 1) != -1 || getWorld()->penelopeHere(getX(), getY() - SPRITE_HEIGHT, 10)) {
                if (randInt(1, 3) == 1) {
                    getWorld()->addActor(new Vomit(getWorld(), 4, getX(), getY() - SPRITE_HEIGHT));
                    getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                    return;
                }
            }
            break;
    }
    // update direction
    if (movement_plan == 0) {
        movement_plan = randInt(3, 10);
        int new_dir = randInt(0, 3);
        setDirection(new_dir * 90);
    }
    // move to new place
    int newx = getX();
    int newy = getY();
    switch (dir) {
        case up:
            newy += 1;
            break;
        case down:
            newy -= 1;
            break;
        case right:
            newx += 1;
            break;
        case left:
            newx -= 1;
            break;
    }
    if (getWorld()->walkable(newx, newy, getX(), getY())) {
        moveTo(newx, newy);
        movement_plan--;
    }
    else {
        movement_plan = 0;
    }
    is_paralyzed = true;
}

void DumbZombie::doSomething() {
    if (!isAlive()) {
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
        //getWorld()->increaseScore(1000);
    }
    Zombie::doSomething();
}

void DumbZombie::dropVaccine() {
    int x = getX();
    int y = getY();
    int rand = randInt(1, 10);
    if (rand == 5) {
        rand = randInt(1, 4);
        switch (rand) {
            case 1:
                y = y + SPRITE_HEIGHT;
                break;
            case 2:
                y = y - SPRITE_HEIGHT;
                break;
            case 3:
                x = x + SPRITE_WIDTH;
                break;
            case 4:
                x = x - SPRITE_WIDTH;
                break;
        }
        if (!getWorld()->objectHere(x, y)) {
            getWorld()->addActor(new Vaccine(getWorld(), IID_VACCINE_GOODIE, getX(), getY()));
        }
    }
}

void SmartZombie::doSomething() {
    if (!isAlive()) {
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
        //getWorld()->increaseScore(2000);
    }
    Zombie::doSomething();
}

void Vomit::doSomething() {
    if (!is_alive) {
        return;
    }
    else if (alive_tick == 2) {
        is_alive = false;
        return;
    }
    else {
        getWorld()->tryToInfectSomeone(getX(), getY());
    }
    alive_tick++;
}


void GasCan::doSomething() {
    if (!isAlive()) {
        return;
    }
    if (getWorld()->penelopeHere(getX(), getY(), 0)) {
        getWorld()->increaseScore(50);
        setToDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->gotGoodies(1);
    }
}

void Vaccine::doSomething() {
    if (!isAlive()) {
        return;
    }
    if (getWorld()->penelopeHere(getX(), getY(), 0)) {
        getWorld()->increaseScore(50);
        setToDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->gotGoodies(2);
    }
}

void LandmineGoodies::doSomething() {
    if (!isAlive()) {
        return;
    }
    if (getWorld()->penelopeHere(getX(), getY(), 0)) {
        getWorld()->increaseScore(50);
        setToDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->gotGoodies(3);
    }
}

void Pit::doSomething() {
    int score = 0;
    if (getWorld()->penelopeHere(getX(), getY(), 0)) {
        getWorld()->penelopeDead();
    }
    int id = getWorld()->characterHere(getX(), getY(), 0, 1);
    if (id != -1) {
        score = getWorld()->actorDead(id);
        getWorld()->increaseScore(score);
        getWorld()->playSound(SOUND_CITIZEN_DIE);
        getWorld()->decCitizen();
    }
    id = getWorld()->characterHere(getX(), getY(), 0, 2);
    if (id != -1) {
        score = getWorld()->actorDead(id);
        getWorld()->increaseScore(score);
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
    }
}

void Flame::doSomething() {
    int score = 0;
    if (!is_alive) {
        return;
    }
    if (m_tick == 2) {
        is_alive = false;
        return;
    }
    if (getWorld()->penelopeHere(getX(), getY(), 0)) {
        getWorld()->penelopeDead();
    }
    int id = getWorld()->characterHere(getX(), getY(), 0, 1);
    if (id != -1) {
        score = getWorld()->actorDead(id);
        getWorld()->increaseScore(score);
        getWorld()->playSound(SOUND_CITIZEN_DIE);
        getWorld()->decCitizen();
    }
    id = getWorld()->characterHere(getX(), getY(), 0, 2);
    if (id != -1) {
        score = getWorld()->actorDead(id);
        getWorld()->increaseScore(score);
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
    }
    id = getWorld()->goodieHere(getX(), getY(), 0);
    if (id != -1) {
        getWorld()->actorDead(id);
    }
    m_tick++;
}

void Landmine::doSomething() {
    if (getWorld()->flamableHere(getX(), getY(), 0)) {
        explode(getX(), getY());
    }
    if (!is_alive) {
        return;
    }
    if (inactive) {
        m_saftytick--;
        if (m_saftytick == 0) {
            inactive = false;
        }
        return;
    }
    if (!inactive) {
        if (getWorld()->penelopeHere(getX(), getY(), 0) || getWorld()->characterHere(getX(), getY(), 0, 1) != -1 || getWorld()->characterHere(getX(), getY(), 0, 2) != -1) {
            explode(getX(), getY());
        }
    }
}

void Landmine::explode(int x, int y) {
    is_alive = false;
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    for (int i = -1; i <= 1; i++) {
        y = getY() + i * SPRITE_HEIGHT;
        for (int j = -1; j <= 1; j++) {
            x = getX() + j * SPRITE_WIDTH;
            getWorld()->addActor(new Flame(getWorld(), IID_FLAME, x, y, up));
        }
    }
    getWorld()->addActor(new Pit(getWorld(), IID_PIT, getX(), getY()));
}
