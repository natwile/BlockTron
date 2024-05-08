#include "Memory.cpp"
#include <chrono>
#include <thread>
#include <time.h>
using namespace std::chrono_literals;
/* Design and Purpose */
/*
- The purpose of some of these functions are to generalize certain actions to not repeat implementations.
    -> This applies to generalPrint() for cutscenes.
- Another purpose of these functions is to take some weight off the front end in terms of implementation.
    -> This applies to The LuckyBlockFunction() and the enemy class with the fight member.
- Exceptions used within the frontend can also be located here.
*/

const int maxHealth = 3; //controls enemys random max health

/* Exception Classes */
#include <stdexcept>
class Win : public std::exception {
public:
  const char *what() { return "Win Condition"; }
};

class Loss : public std::exception {
public:
  const char *what() { return "Loss Condition"; }
};

class Save : public std::exception {
public:
  const char *what() { return "Save Condition"; }
};

/* Functions */

/*
- This functions allows for cutscenes to be printed to screen with specific constructors
- This acts as a helper function to other functions such as the Lucky Block Function
*/
void generalPrint(int sizeScreen = 10, int sizeString = 3, std::string strings[] = nullptr) {
    for (int i = 0; i < (sizeScreen * 3) + 2; i++) {
            std::cout << "-";
        }
        std::cout << std::endl;
        for (int i = 0; i < sizeScreen; i++) { // top to bottom
            for (int j = 0; j < 32; j++) { // left to right
                if (j == 0 || j == 31) {std::cout << "|";} 
                else if (i < ((sizeScreen / 2)) + sizeString - (sizeString/2) // addition and subtraction deals with odd numbers
                        && i > ((sizeScreen / 2)) - sizeString + (sizeString/2) // no equal signs deals with odd numbers
                        && j > (((sizeScreen * 3) + 2) / 2) - sizeString + (sizeString/2)
                        && j < (((sizeScreen * 3) + 2) / 2) + sizeString - (sizeString/2)) { 
                    int pos = (i + 2) % (((sizeScreen / 2)) - sizeString + 1);
                    std::cout << strings[pos];
                    j += (sizeString - 1);
                }
                else {std::cout << " ";}
            }
            std::cout << std::endl;
        }
        for (int i = 0; i < 32; i++) {
            std::cout << "-";
        }
}

/* 
- This function allows for lucky block rolling screen and returns the reward it lands on
- Randomizes start of array and amount of movement
*/
std::string LuckyBlockFunction(bool testLuck) {
    std::string outcomes[6] = { //return types
        "HP", "SCORE", "NULL",
        "-HP", "-SCORE", "BONUS"
    };
    std::string art[18] { //art associated with outcomes in order of array definition
        "^_^", "\\ /", " V ",
        " ^ ", "< >", " W ",
        "\\ /", " X ", "/ \\",
        "^_^", "\\X/", " V ",
        " ^ ", "<X>", " W ",
        "$$$","$$$","$$$"
    };
    int roll = std::rand() % 6; //random number of rolls
    int draw = std::rand() % 2; //special case and used in later definition
    int start = std::rand() % 6; //start position inside the array
    if (testLuck && draw) {roll += 1;}
    else if ((testLuck || draw) && roll != 0) {roll -= 1;}
    else {roll = 2;}
    for (int k = 0; k < roll; k++) {
        draw = (start + k) % 6; //assigns where it is in array and used to find print position
        std::string print[3] = {art[(draw*3)],art[(draw*3)+1],art[(draw*3)+2]};
        generalPrint(10,3,print);
        std::cout << std::endl << std::endl << std::endl << std::endl;
        std::this_thread::sleep_for(1s);
    }
    return outcomes[draw];
}

/*
This class will be used within the friend end with built in functions for controlling fight screens
*/
class Enemy {
private:
    int enemyHealth;
    bool firstAttack;
public:
    Enemy() {enemyHealth = (std::rand() % maxHealth) + 1; firstAttack = std::rand() % 2;}
    int fight(int playerHealth) { //returns remaining health of player
        std::string defense[3] = {" \\ ", "<--"," / "};
        std::string attack[3] = {" / ", "-->"," \\ "};
        while (enemyHealth > 0) {
            std::cout << "Your HP: " << playerHealth << "     Enemy Health: " << enemyHealth << std::endl;
            if (firstAttack) {generalPrint(10,3,defense); playerHealth -= 1;}
            else {generalPrint(10,3,attack); enemyHealth -= 1;}
            if (playerHealth <= 0) {throw Loss();}
            firstAttack = !firstAttack;
            std::cout << std::endl << std::endl << std::endl << std::endl;
            std::this_thread::sleep_for(1s);
        }
        std::cout << "Your HP: " << playerHealth << "     Enemy Health: " << enemyHealth << std::endl;
        std::string win[3] = {"  /", "\\/ ", "   "};
        generalPrint(10,3,win);
        std::cout << std::endl;
        return playerHealth;
    }
};

