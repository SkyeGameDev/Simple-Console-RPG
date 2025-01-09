#include "game.h"
#include "components.h"
#include <conio.h>
#include <iostream>

using namespace std;

entity::entity()
{
    active = true;
}
entity::~entity() {}
playerCharacter::playerCharacter()
{
    HP = 100;
    maxHP = HP;
    ATK = 7;
    DEF = 7;
    MANA = 20;
    maxMANA = MANA;
    maxEnergy = 3;
    EXP = 0;
    nextLevelReq = 100;
    level = 1;
    Fleeing = false;
}
playerCharacter::~playerCharacter() {}
NPC::NPC(int difficultyScale)
{
    switch ((rand() % 5) + 1)
    {
    case 1: //Strength
        role = "Warrior";
        HP = ((rand() % 8) + 7 + (2 * difficultyScale)) * 10; //Base range of 70 to 140
        ATK = (rand() % 6) + 10; //Range of 10 to 15
        DEF = (rand() % 6) + 4; //Range of 4 to 9
        MANA = 0;
        break;
    case 2: //Magic
        role = "Mage";
        HP = ((rand() % 7) + 2.5 + difficultyScale) * 10; //Base range of 25 to 85
        ATK = (rand() % 3) + 1; //Range of 1 to 3
        DEF = (rand() % 3) + 1; //Range of 1 to 3
        MANA = ((rand() % 5) + 2) * 10; //Range of 20 to 60
        break;
    case 3: //HP
        role = "Brute";
        HP = ((rand() % 11) + 10 + (3 * difficultyScale)) * 10; //HP range for NPCs is 100 to 200
        ATK = (rand() % 4) + 3; //Range of 3 to 6
        DEF = rand() % 2; //Range of 0 to 1
        MANA = (rand() % 36) + 5; //Range of 5 to 40
        break;
    case 4: //Defense
        role = "Knight";
        HP = ((rand() % 6) + 5 + (2 * difficultyScale)) * 10; //HP range for NPCs is 50 to 100
        ATK = (rand() % 6) + 4; //Range of 4 to 9
        DEF = (rand() % 6) + 10; //Range of 10 to 15
        MANA = 0;
        break;
    case 5: //All-rounder
        role = "Fighter";
        HP = 100 + (2 * difficultyScale);
        ATK = 7;
        DEF = 7;
        MANA = 20;
        break;
    default:
        cout << "Error: If you are reading this them something has gone wrong. \n";
        break;
    }

    maxHP = HP;
    maxMANA = MANA;
    name = assignName();
}
NPC::~NPC() {}


void entity::displayHealth() const
{
    int healthPercentage;
    healthPercentage = int((HP / maxHP) * 10) % 11;

    for (int i = 0; i < healthPercentage; i++)
    {
        cout << "Û"; //Due to how the Windows CMD encodes characters, this is the character code that displays a █ in the terminal
    }
    for (int i = 0; i < 10 - healthPercentage; i++)
    {
        cout << "±"; //Read above. Correct character  is ▒
    }
}

bool entity::isAlive()
{
    if (HP <= 0)
    {
        active = false;
        return false;
    }
    else return true;
}

float entity::damage() //Magic damage
{
    float damageDealt;
    damageDealt = (0.5F * DEF) + 25;
    if (damageDealt < 0) damageDealt = 0;
    HP -= damageDealt;

    return damageDealt;
}
float entity::damage(float strength) //Physical damage
{
    float damageDealt;
    damageDealt = (-0.5F * DEF) + 2 * strength;
    HP -= damageDealt;

    return damageDealt;
}


void playerCharacter::displayStats() const
{
    cout << name;
    if (active == false) cout << " - Deceased:\n";
    cout << ": \nHealth: ";
    displayHealth();
    cout << " - " << HP << "/" << maxHP;
    cout << "\nATK: " << ATK;
    cout << "\nDEF: " << DEF;
    cout << "\nMANA: " << MANA << "/" << maxMANA;
    cout << "\nLevel: " << level;
    cout << "\nEXP: " << EXP << "/" << nextLevelReq;
    cout << "\n\n";
}

float playerCharacter::Attack(NPC& target) const
{
    return target.damage(ATK * 1.2);
}
float playerCharacter::spellFireball(NPC& target)
{
    if (MANA >= 10)
    {
        MANA -= 10;
        return target.damage();
    }
    else
    {
        return -1;
    }
}
float entity::spellHeal()
{
    if (MANA >= 20)
    {
        MANA -= 20;
        if (HP < maxHP - 25)
        {
            HP += 25;
        }
        else if (HP < maxHP && HP + 25 > maxHP)
        {
            HP = maxHP;
        }
        return -1;
    }
    else
    {
        return -2;
    }
}
float playerCharacter::Flee()
{
    int fleeChance = 1;
    fleeChance = rand() % 3;

    if (fleeChance == 1)
    {
        Fleeing = true;
        HP += maxHP * 0.25;
        return 1;
    }
    else
    {
        cout << "Flee unsuccessful\n";
        return 0;
    }

}

void playerCharacter::giveEXP(int sumEXP)
{
    EXP += sumEXP;
    cout << "\nYou gained " << sumEXP << " EXP.\n";
}
void playerCharacter::levelUp()
{
    while (EXP >= nextLevelReq)
    {
        EXP -= nextLevelReq;
        nextLevelReq *= 1.35;
        level += 1;
        maxHP += 5;
        navigateLevelupScreen();
    }
    HP = maxHP;
    MANA = maxMANA;
}
void playerCharacter::navigateLevelupScreen()
{
    bool validKey = false;
    char c;
    int hovered = 1;
    displayLevelupScreen(hovered);
    do
    {
        c = _getch();
        switch (toupper(c))
        {
        case 'W':
            if (hovered > 1) hovered -= 1;
            else hovered = 4;
            displayLevelupScreen(hovered);
            break;
        case 'S':
            if (hovered < 4) hovered += 1;
            else hovered = 1;
            displayLevelupScreen(hovered);
            break;
        case 'Z':
            validKey = true;
            switch (hovered) {
            case 1: maxHP += 5;
                break;
            case 2: ATK += 1;
                break;
            case 3: DEF += 1;
                break;
            case 4: maxMANA += 5;
                break;
            default: break;}
            break;
        default:
            validKey = false;
            break;
        }
    } while (!validKey);
}
void playerCharacter::displayLevelupScreen(int hovered) const
{
    system("cls");

    cout << "========== You leveled up! ==========\n\n";
    cout << "Level: " << level - 1 << " --> " << level << "\n";
    cout << "HP: " << maxHP - 5 << " --> " << maxHP << "\n";
    if (level % 5 == 0)
    {
        cout << "The game gets harder...\n";
    }
    cout << "\nChoose a stat to increase [W/S]\n";

    switch (hovered)
    {
    case 1:
        cout << "\n| HP: " << maxHP << " --> " << maxHP + 5 << " |";
        cout << "\nATK: " << ATK << " --> " << ATK + 1;
        cout << "\nDEF: " << DEF << " --> " << DEF + 1;
        cout << "\nMANA: " << maxMANA << " --> " << maxMANA + 5 << "\n\n";
        break;
    case 2:
        cout << "\nHP: " << maxHP << " --> " << maxHP + 5;
        cout << "\n| ATK: " << ATK << " --> " << ATK + 1 << " |";
        cout << "\nDEF: " << DEF << " --> " << DEF + 1;
        cout << "\nMANA: " << maxMANA << " --> " << maxMANA + 5 << "\n\n";
        break;
    case 3:
        cout << "\nHP: " << maxHP << " --> " << maxHP + 5;
        cout << "\nATK: " << ATK << " --> " << ATK + 1;
        cout << "\n| DEF: " << DEF << " --> " << DEF + 1 << " |";
        cout << "\nMANA: " << maxMANA << " --> " << maxMANA + 5 << "\n\n";
        break;
    case 4:
        cout << "\nHP: " << maxHP << " --> " << maxHP + 5;
        cout << "\nATK: " << ATK << " --> " << ATK + 1;
        cout << "\nDEF: " << DEF << " --> " << DEF + 1;
        cout << "\n| MANA: " << maxMANA << " --> " << maxMANA + 5 << " |\n\n";
        break;
    default:
        cout << "\nHP: " << maxHP << " --> " << maxHP + 5;
        cout << "\nATK: " << ATK << " --> " << ATK + 1;
        cout << "\nDEF: " << DEF << " --> " << DEF + 1;
        cout << "\nMANA: " << maxMANA << " --> " << maxMANA + 5 << "\n\n";
        break;
    }
    cout << "[Z] to confirm\n";
}

int playerCharacter::checkLevel() const
{
    return level;
}


string NPC::assignName()
{
    string names[10] = { "Eleanora", "Gregory", "Randall", "Julius", "Finley",
    "Madlyn", "Kristopher", "Mathew", "Cornelia", "Gillian" };

    return names[rand() % 10];
}
void NPC::displayStats() const
{
    cout << name;
    if (active == false) cout << " - Deceased:\n";
    cout << ": \nHealth: ";
    displayHealth();
    cout << " - " << HP << "/" << maxHP;
    cout << "\nClass: " << role;
    cout << "\nATK: " << ATK;
    cout << "\nDEF: " << DEF;
    cout << "\nMANA: " << MANA << "/" << maxMANA;
    cout << "\n\n";
}
int NPC::sumOfStats() const
{
    int sumEXP;

    sumEXP = maxHP + ATK + DEF + MANA;
    if (role == "Mage") sumEXP /= 2;

    return sumEXP;
}

float NPC::enemyTurn(playerCharacter& pc)
{
    float actionResult;
    if (role == "Mage" && rand() % 5 != 1)
    {
        if (MANA >= 10)
        {
            actionResult = pc.damage();
            MANA -= 10;
        }
        else
        {
            actionResult = pc.damage(ATK);
        }
    }
    else if (role == "HP" && rand() % 5 == 1)
    {
        actionResult = spellHeal();
        if (actionResult == -2) actionResult = pc.damage(ATK);
    }
    else
    {
        actionResult = pc.damage(ATK);
    }

    return actionResult;
}