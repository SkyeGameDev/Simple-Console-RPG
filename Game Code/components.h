#pragma once
#include <string>

class entity
{
public:
    //Constructors
    entity();
    ~entity();
    //Properties
    std::string name;
    //Methods
    void displayHealth() const;
    float damage();
    float damage(float);
    bool isAlive();
    float spellHeal();
protected:
    //Properties
    float HP, maxHP, ATK, DEF;
    int MANA, maxMANA;
    bool active;
};

//Defined with no members here, as both classes reference each other in parameters
class playerCharacter;
class NPC;

class playerCharacter : public entity
{
public:
    //Constructors
    playerCharacter();
    ~playerCharacter();
    //Methods
    void displayStats() const;
    void giveEXP(int);
    void levelUp();
    void navigateLevelupScreen();
    void displayLevelupScreen(int) const;
    float Attack(NPC&) const;
    float spellFireball(NPC&);
    float Flee();
    int checkLevel() const;
    //Properties
    bool Fleeing;
private:
    int maxEnergy;
    int EXP, nextLevelReq;
    int level;
};

class NPC : public entity
{
public:
    //Constructors
    NPC(int);
    ~NPC();
    void displayStats() const;
    int sumOfStats() const;
    float enemyTurn(playerCharacter&);
    std::string assignName();
private:
    //Properties
    std::string role;
};