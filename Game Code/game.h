#pragma once

//Constants
constexpr auto Title = "CI411 assignement working title";

class Game
{
public:
    bool replay;
    Game();
    ~Game();
    void mainLoop();
    void startGame();
    void gameOver(bool);
    void battle();
    void waitForInput();
    void enemyTurn();
    void drawBattleArena(int);
    void navigateBattleScreen(int&);
    bool checkGameState();
private:
    int score;
    bool gameRunning;
};

