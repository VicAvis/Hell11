#include <algorithm>
#include "Monster.h"
#include "Manager.h"

bool Monster::isActive() const { return getHealth() != 0; }

Monster::Monster(){
    this->state = new FarFromHeroState(2);
    setSpeed(6);
    setProtection(1);
    setDamage(6);
    setHealth(10);
    setX(0);
    setY(0);
}
Monster::~Monster() {
    delete state;
}
void Monster::setState(MonsterState* newState) {
    delete state;
    state = newState;
}
void Monster::ChangeState(HeroBase& hero) {
    if ((getX() == hero.getX() - 1 || getX() == hero.getX() + 1) && (getY() == hero.getY() - 1 || getY() == hero.getY() + 1)) {
        delete state;
        state = new AttackState(0);
        return;
    }
    if ((getX() == hero.getX() && (getY() == hero.getY() - 1 || getY() == hero.getY() + 1)) || (getY() == hero.getY() && (getX() == hero.getX() - 1 || getX() == hero.getX() + 1))) {
        delete state;
        state = new NearHeroState(1);
        return;
    }
    delete state;
    state = new FarFromHeroState(2);
}
void Monster::Move(HeroBase& hero, Field* gameField, Monster& monster) {
    state->Move(hero, gameField, monster);
}
void Monster::Attack(HeroBase& hero, Monster& monster) {
    state->Attack(hero, monster);
}
//MonsterState::MonsterState(Monster& newmonster) : monster() {
//    this->monster = newmonster;
//}
void MonsterState::Attack(HeroBase& hero, Monster& monster) {}
void MonsterState::Move(HeroBase& hero, Field* gameField, Monster& monster) {}

NearHeroState::NearHeroState(int sn) : MonsterState(sn) {}

void NearHeroState::Attack(HeroBase& hero, Monster& monster) {

}
void NearHeroState::Move(HeroBase& hero, Field* gameField, Monster& monster) {
    if (monster.getX() == hero.getX())
    {
        if (gameField->IsCellFree(monster.getX() + 1, monster.getY())){
            gameField->moveUnit(monster, monster.getX() + 1, monster.getY());
        }
        else {
            gameField->moveUnit(monster, monster.getX() - 1, monster.getY());
        }
    }
    else {
        if (gameField->IsCellFree(monster.getX() , monster.getY() + 1)) {
            gameField->moveUnit(monster, monster.getX(), monster.getY() + 1);
        }
        else {
            gameField->moveUnit(monster, monster.getX(), monster.getY() - 1);
        }
    }
    monster.setState(new AttackState(0));
}

FarFromHeroState::FarFromHeroState(int sn) : MonsterState(sn) {}

void FarFromHeroState::Attack(HeroBase& hero, Monster& monster) {

}
void FarFromHeroState::Move(HeroBase& hero, Field* gameField, Monster& monster) {
    int delX = hero.getX() - monster.getX(), delY = hero.getY() - monster.getY();
    if (std::abs(delX) > std::abs(delY)) {
        int newX = monster.getX() + (delX > 0 ? 1 : (delX < 0 ? -1 : 0));
        // if (gameField->isCellHasObstacle(newX, monster.getY())) {
        //     return;
        // }
        gameField->moveUnit(monster, newX, monster.getY());
    }
    else {
        int newY = monster.getY() + (delY > 0 ? 1 : (delY < 0 ? -1 : 0));
        gameField->moveUnit(monster, monster.getX(), newY);
    }
    monster.ChangeState(hero);
}
AttackState::AttackState(int sn) : MonsterState(sn) {}

void AttackState::Attack(HeroBase& hero, Monster& monster){
    int monsterDistance = std::max(std::abs(hero.getX() - monster.getX()), std::abs(hero.getY() - monster.getY()));
    int totalAttack;
    // порахувати загальну атаку тих монстрів, які знаходяться на 1 клітинку від героя
    if (monster.isActive()) {
        if (monsterDistance <= hero.getDistance()) {
            totalAttack = monster.getDamage();
        }
        else {
            totalAttack = 0;
        }
    }
    else {
        return;
    }
    int damage;
    if (totalAttack > hero.getProtection()) {
        damage = totalAttack - hero.getProtection();
    }
    else {
        damage = 0;
    }
    hero.reduceHealth(damage);
    if(hero.getProtection()) hero.setProtection(hero.getProtection() - 1);
}
void AttackState::Move(HeroBase& hero, Field* gameField, Monster& monster) {
    //AttackState::Attack(hero, monster);
}

/*void Monster::MonsterMove(HeroBase& hero, Monster& monsters, Field* gameField) {
    int mX = getX(), mY = getY();
    int hX = hero.getX(), hY = hero.getY();

    if (hX == mX && hY == mY) return; //error - monstr = hero

    // перевірка на зону атаки - старт
    if ((mX == hX - 1 || mX == hX + 1) && (mY == hY - 1 || mY == hY + 1)) {
        MonsterAttack(hero);
        return;
    }
    // перевірка на зону атаки - фініш

    // перевірка на розміщення поряд з героєм - старт
    if (mX == hX && (mY == hY - 1 || mY == hY + 1)) {
        gameField->moveUnit(*this, getX(), getY() + 1);
        return;
    }
    else if (mY == hY && (mX == hX - 1 || mX == hX + 1)) {
        gameField->moveUnit(*this, getX() + 1, getY());
        return;
    }
    // перевірка на розміщення поряд з героєм - фініш

    // всі інші випадки - старт
    int delX = hX - mX, delY = hY - mY;
    if (std::abs(delX) > std::abs(delY)) {
        int newX = getX() + (delX > 0 ? 1 : (delX < 0 ? -1 : 0));
        if (gameField->isCellHasObstacle(newX, mY)) {
            return;
        }
        gameField->moveUnit(*this, newX, mY);
    }
    else {
        gameField->moveUnit(*this, getX(), getY() + (delY > 0 ? 1 : (delY < 0 ? -1 : 0)));
    }
    // всі інші випадки - фініш
}*/
/*void Hydra::MonsterAttack(HeroBase& hero, Monster& monsters) {
    //for (auto& monster : monsters) {
    int totalAttack;
    // порахувати загальну атаку тих монстрів, які знаходяться на 1 клітинку від героя
    if (isActive()) {
        if ((getX() == hero.getX() - 1 || getX() == hero.getX() + 1) && (getY() == hero.getY() - 1 || getY() == hero.getY() + 1)) {
            totalAttack = getDamage();
        }
        else {
            totalAttack = 0;
        }
    }
    else {
        return;
    }

    int heroDefense = hero.getProtection();
    int damage;

    if (totalAttack > heroDefense) {
        damage = totalAttack - heroDefense;
    }
    else {
        damage = 0;
    }
    hero.reduceHealth(damage);
    //}
    hero.setProtection(hero.getProtection() - 1);
}
void Hydra::MonsterMove(HeroBase& hero, Monster& monsters, Field* gameField) {

}
void Goblin::MonsterAttack(HeroBase& hero, Monster& monsters) {
    //for (auto& monster : monsters) {
    int totalAttack;
    // порахувати загальну атаку тих монстрів, які знаходяться на 1 клітинку від героя
    if (isActive()) {
        if ((getX() == hero.getX() && (getY() == hero.getY() - 1 || getY() == hero.getY() + 1)) || (getY() == hero.getY() && (getX() == hero.getX() - 1 || getX() == hero.getX() + 1))) {
            totalAttack = getDamage();
        }
        else {
            totalAttack = 0;
        }
    }
    else {
        return;
    }

    int heroDefense = hero.getProtection();
    int damage;

    if (totalAttack > heroDefense) {
        damage = totalAttack - heroDefense;
    }
    else {
        damage = 0;
    }
    hero.reduceHealth(damage);
    //}
}
*/
