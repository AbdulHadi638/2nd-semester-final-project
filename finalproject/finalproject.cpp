#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

////`````````````````SAIF```````````````````````` ////

// === Game Configuration Constants ===
const int screenwdth = 70;     // Width of the game screen
const int LEFT_BOUNDARY = 18;
const int RIGHT_BOUNDARY = 50;
const int carwdth = 4;         // Width of the player's car
const int carhgt = 4;          // Height of the player's car
const int enemyno = 2;         // Number of enemy cars on screen


// === Game State Variables ===
int carX = screenwdth / 2;     // Horizontal position of our car
int score = 0;                 //intialising score

int enemyX[enemyno];           // X positions of enemy cars
int enemyY[enemyno];           // Y positions of enemy cars
bool enemyActive[enemyno] = { true, false }; // Activation status of enemy

// === Player Car Representation ===
// The car is designed using ASCII characters
char car[carhgt][carwdth] =
{
    {' ', '±', '±', ' '},
    {'±', '±', '±', '±'}
};

// **** Utility Function: Move cursor to (x, y) in console *****
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// *****Render the player's car on screen at its current position ***
void drawCar() {
    for (int i = 0; i < carhgt; ++i)
    {
        for (int j = 0; j < carwdth; ++j)
        {
            gotoxy(carX + j, 22 + i);
            cout << car[i][j];
        }
    }
}

//****** Remove the car's current drawing to allow redrawing ******
void removecar() {
    for (int i = 0; i < carhgt; ++i)
    {
        for (int j = 0; j < carwdth; ++j)
        {
            gotoxy(carX + j, 22 + i);
            cout << ' ';
        }
    }
}

// ****** Generate a random horizontal position for an enemy car********
int randomX()
{
    return 18 + rand() % 33; // Ensure enemy stays within road boundaries
}

// ****** Render an enemy car based on its position *********

void drawEnemy(int i)
{
    if (enemyActive[i])
    {
        gotoxy(enemyX[i], enemyY[i]);     cout << "";
        gotoxy(enemyX[i], enemyY[i] + 1); cout << " ** ";
        gotoxy(enemyX[i], enemyY[i] + 2); cout << "";
        gotoxy(enemyX[i], enemyY[i] + 3); cout << " ** ";
    }
}

// ********* Remove an enemy car from its previous position **********
void eraseEnemy(int i)
{
    if (enemyActive[i])
    {
        for (int j = 0; j < 4; ++j)
        {
            gotoxy(enemyX[i], enemyY[i] + j);
            cout << "    ";
        }
    }
}

// ********** Reset an enemy car to the top of the screen with new X position *********
void resetEnemy(int i)
{
    eraseEnemy(i);
    enemyY[i] = 0;
    enemyX[i] = randomX();
}

///^^^^^^^^^^^^^^^^^^^^^SAIF^^^^^^^^^^^^^^^^^^^^^^/////

// ******* Check if the player's car has collided with enemy i ********
bool isCollision(int i)
{
    if (enemyY[i] + 4 >= 22 && enemyY[i] <= 26)
    { // Y-axis overlap
        if (enemyX[i] + 4 > carX && enemyX[i] < carX + carwdth)
        { // X-axis overlap
            return true;
        }
    }
    return false;
}



// ****** Display the player's current score on screen **********
void updateScore()
{
    gotoxy(screenwdth + 5, 5);
    cout << "Score: " << score;
}

// ********* Display the start menu with user options *******
void showMenu()
{
    system("cls");
    cout << "===== CAR GAME =====\n\n";
    cout << "1. Play\n";
    cout << "2. Exit\n\n";
    cout << "Select your  option: ";
}

// ********** Main gameplay loop  *******
void play()
{
    system("cls");
    srand((unsigned)time(0)); // Seed random for enemy X positions

    // Initialize enemy positions
    for (int i = 0; i < enemyno; ++i)
    {
        enemyX[i] = randomX();
        enemyY[i] = 0;
    }

    // Display controls and game UI
    gotoxy(screenwdth + 5, 2); cout << "CAR GAME";
    gotoxy(screenwdth + 5, 4); cout << "Controls:";
    gotoxy(screenwdth + 5, 6); cout << "A - Left";
    gotoxy(screenwdth + 5, 7); cout << "D - Right";
    updateScore();

    // === Main Game Loop ===
    while (true) {
        // --- Handle Input ---
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'a' || ch == 'A')
            {
                if (carX > 18) carX -= 4; // Move left
            }
            else if (ch == 'd' || ch == 'D')
            {
                if (carX < 50) carX += 4; // Move right
            }
            else if (ch == 27) { // ESC key to quit
                break;
            }
        }

        // --- Draw Cars ---
        drawCar();
        for (int i = 0; i < enemyno; ++i) drawEnemy(i);

        // --- Collision Check ---
        for (int i = 0; i < enemyno; ++i)
        {
            if (isCollision(i))
            {
                gotoxy(20, 10); cout << "GAME OVER!";
                gotoxy(20, 11); cout << "Your final score: " << score;
                gotoxy(20, 13); cout << "Play again? (Y/N): ";
                char ch;
                do
                {
                    ch = _getch();
                    ch = toupper(ch);

                } while (ch != 'Y' && ch != 'N');

                if (ch == 'Y')
                {
                    score = 0;
                    carX = screenwdth / 2;
                    enemyActive[0] = true;
                    enemyActive[1] = false;
                    system("cls");
                    return play();  // Restart game
                }
                else
                {
                    exit(0); // Quit program

                }
            }
        }

        // --- Delay for smooth motion ---

        Sleep(60);

        // --- Erase Cars ---

        removecar();
        for (int i = 0; i < enemyno; ++i) eraseEnemy(i);

        // --- Move Enemies ---

        for (int i = 0; i < enemyno; ++i)
        {
            if (enemyActive[i]) enemyY[i]++;
        }

        // --- Activate second enemy later ---

        if (enemyY[0] == 10 && !enemyActive[1])
            enemyActive[1] = true;

        // --- Reset enemies and update score ---
        for (int i = 0; i < enemyno; ++i)
        {
            if (enemyY[i] > 22)
            {
                resetEnemy(i);
                score++;
                updateScore();
            }
        }
    }
}

// ********* Entry point of the program *******
int main()
{
    while (true)
    {
        showMenu();
        char choice = _getch();
        if (choice == '1')
        {
            score = 0;
            enemyActive[0] = true;
            enemyActive[1] = false;
            play();

        }
        else if (choice == '2')
        {
            break;
        }
    }


    return 0;

}

//******************************************************////