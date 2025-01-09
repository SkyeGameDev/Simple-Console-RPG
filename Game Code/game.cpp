#include "game.h"
#include "components.h"
#include <conio.h>
#include <iostream>
using namespace std;

//Global Variable definitions
playerCharacter* pc = nullptr;
NPC* enemyNPC = nullptr;


//Game class
//Constructors
Game::Game()
{
    startGame();
}
Game::~Game()
{
    delete pc;
    delete enemyNPC;
}

void Game::mainLoop()
{
    while (gameRunning)
    {
        checkGameState();
        battle();
    }
}

void Game::battle()
{
    int hovered = 1;
    enemyNPC = new NPC(pc->checkLevel() / 5);
    while (pc->isAlive() && enemyNPC->isAlive())
    {
        drawBattleArena(hovered);
        navigateBattleScreen(hovered);
        waitForInput();
        if (pc->Fleeing == true) break;
        if (enemyNPC->isAlive() == false) break;
        enemyTurn();
    }

    if (pc->Fleeing == true) pc->Fleeing = false; //if the player is fleeing, skips other checks
    else if (!pc->isAlive()) //if the player is dead
    {
        gameOver(false);
    }
    else if (!enemyNPC->isAlive()) //if the opponent is dead
    {
        pc->giveEXP(enemyNPC->sumOfStats());
        waitForInput();
        pc->levelUp();

        checkGameState();
    }
}

void Game::startGame()
{
    srand(static_cast<unsigned int>(time(0))); //Seeds the randomisation
    gameRunning = true;

    cout << "========== " << Title << " ==========\n";
    cout << "The goal of this game is to reach level 10 from defeating aggressors\n\n";
    pc = new playerCharacter;
    cout << "Welcome. Please enter your name: ";
    cin >> pc->name;
    cout << "\n";
}
void Game::gameOver(bool winState)
{
    gameRunning = false;

    system("cls");
    cout << "========== Game Over ==========\n";
    if (winState) //Player win
    {
        cout << "========== You win! ==========\n";
    }
    else
    {
        cout << "========== You have lost ==========\n";
    }

    score = pc->checkLevel();
    cout << "Final score: " << score;
    cout << "\n\nTry again? [Y/N]\n";

    char c;
    bool validKey = false;
    do
    {
        c = _getch();

        if (toupper(c) == 'Y')
        {
            replay = true;
            validKey = true;
        }
        else if (toupper(c) == 'N')
        {
            cout << "Thanks for playing!";
            replay = false;
            validKey = true;
        }
    } while (!validKey);
}

bool Game::checkGameState()
{
    if (pc->isAlive() == false)
    {
        gameRunning = false;
        gameOver(false);
        return false;
    }
    else
    {
        gameRunning = true;
        if (pc->checkLevel() >= 10)
        {
            score = pc->checkLevel();
            gameOver(true);
        }
        return true;
    }
}

void Game::waitForInput()
{
    cout << "\n=== Press anything to continue === ";
    _getch();
}

void Game::enemyTurn()
{
    float actionResult;
    actionResult = enemyNPC->enemyTurn(*pc);

    drawBattleArena(-1);

    if (actionResult == -1)
    {
        cout << "The enemy healed!";
    }
    else
    {
        cout << "The enemy attacks you for " << actionResult << " damage";
    }
    waitForInput();
}

void Game::drawBattleArena(int hovered)
{
    system("cls");

    cout << "======================== Battle ========================\n";
    if (hovered == 1) cout << "|> Attack <|  Cast Fireball  |  Cast Healing  |  Flee  |\n";
    else if (hovered == 2) cout << "|  Attack  |> Cast Fireball <|  Cast Healing  |  Flee  |\n";
    else if (hovered == 3) cout << "|  Attack  |  Cast Fireball  |> Cast Healing <|  Flee  |\n";
    else if (hovered == 4) cout << "|  Attack  |  Cast Fireball  |  Cast Healing  |> Flee <|\n";
    else cout << "|  Attack  |  Cast Fireball  |  Cast Healing  |  Flee  |\n";
    cout << "================= A + D to move cursor =================\n";
    cout << "===================== Z to select ======================\n\n";

    pc->displayStats();
    enemyNPC->displayStats();
}
void Game::navigateBattleScreen(int& hovered)
{
    bool validKey = false;
    char c;
    do
    {
        c = _getch();
        switch (toupper(c)) {
        case 'A': //Left
            if (hovered > 1) hovered -= 1;
            else hovered = 4;
            drawBattleArena(hovered);
            break;
        case 'D': //Right
            if (hovered < 4) hovered += 1;
            else hovered = 1;
            drawBattleArena(hovered);
            break;
        case 'Z': //Enter
            float actionResult;
            validKey = true;
            switch (hovered) {
            case 1: //Attack
                actionResult = pc->Attack(*enemyNPC);
                drawBattleArena(hovered);
                cout << "You attacked and dealt " << actionResult << " damage";
                break;
            case 2: //Spell
                actionResult = pc->spellFireball(*enemyNPC);
                drawBattleArena(hovered);
                if (actionResult == -1) {
                    cout << "You do not have enough mana, try something else";
                    validKey = false;
                    waitForInput();
                    drawBattleArena(hovered); }
                else cout << "You cast fireball and dealt " << actionResult << " damage";
                break;
            case 3: //Heal
                actionResult = pc->spellHeal();
                drawBattleArena(hovered);
                if (actionResult == -1) cout << "You successfully healed";
                else {
                    cout << "You do not have enough mana, try something else";
                    validKey = false;
                    waitForInput();
                    drawBattleArena(hovered); }
                break;
            case 4: //Flee
                actionResult = pc->Flee();
                if (actionResult) cout << "You successfully fled";
                break;
            }
            break;
        default:
            validKey = false;
            break;
        }
    } while (!validKey);
}