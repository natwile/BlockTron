#include <conio.h>
#include "Functions.cpp"

/*  Design and Purpose  */
/*
-The design of this FrontEnd includes throw/catch for abrupt and game stopping conditions.
-The FrontEnd is designed with an x/y coordinate system with a 1D array for the map conditions.
-The map pointer uses the generate() function to create a 1D array with a specific size.
-Most of the member variables of the FrontEnd class are private to prevent unwanted manipulation.

-The purpose of the print() function is to add a terminal side interface for visual effect.
-The purpose of the movement() and action functions are to allow for movement within the map with specific rules.
-The purpose of newLevel() is to add progression into the game with varying map sizes.
-The purpose of loadGame() and newGame() is to allow for abruptions and continuations within the game, as well as restarting.
*/

const int attackScore = 250; //score modifier used when winning attacks
const int luckyScore = 100; //score modifier for score related lucky blocks
const int luckyHealth = 3; //health modifier for health related lucky blocks
const int blockScore = 3; //score modifier for score related block removals
bool adminActive = true; //This is used for testing purposes and can be disabled with the below constant.
std::string checkLocation[4] = {"left", "up", "right", "down"}; //Used for enemyAI()


/* FrontEnd class for visuals */
class FrontEnd {
private:
    int x; //player x pos
    int y; //player y pos
    int size; //size of the map
    int *map; //generate map as 1D array
    int score; //score of player
    int level; //current level
    
    File file; //file object used for i/o
    int health; //health of player
public: 
    std::string name; //name of player
    Memory memory; //memory object used for i/o
    int enemyLocation; //location of enemy on map
    bool isEnemyAlive = false; // enemy condition
    FrontEnd(int s){  //game creation
        x = 0;
        y = 0;
        size = s;
        health = 5;
        map = generate(size);
        score = 0;
        level = 1;
        file = File();
        name = "";
    }

    void setPosition(int xpos, int ypos) { //sets position coordinates
        x = xpos;
        y = ypos;
    }

    void clear() { //adds a space between prints
        for (int i = 0; i < (size + 8); i++) {
            std::cout << std::endl;
        }
    }

    void print() { //prints screen conditions
        for (int i = 0; i < (size*3) + 2; i++) {
            std::cout << "-";
        }
        std::cout << std::endl;
        for(int i = 0; i < size; i++) {
            std::cout << "|";
            for (int j = 0; j < size; j++) {
                if (i == y && j == x) {std::cout << " X ";}
                else if (map[(i*size) + j] == 3) {std::cout << " $ ";}
                else if (map[(i*size) + j] == 2) {std::cout << " ? ";}
                else if (map[(i*size) + j] == 1) {std::cout << "   ";}
                else if (map[(i*size) + j] == 0) {std::cout << "[ ]";}
                else if (map[(i*size) + j] == 5) {std::cout << "[=]";}
                else if (map[(i*size) + j] == 4 && isEnemyAlive) {std::cout << "{*}";}
            }
            std::cout << "|";
            if (i == 0) {std::cout << "    Name: " << name;}
            else if (i == 1) {std::cout << "    Health: " << health;}
            else if (i == 2) {std::cout << "    Level: " << level;}
            else if (i == 3) {std::cout << "    Score: " << score;}
            else if (i == 4) {std::cout << "    1->(w,a,s,d) : Break Block";}
            else if (i == 5) {std::cout << "    2->(w,a,s,d) : Attack Location";}
            else if (i == 6) {std::cout << "    3 : Save Game";}
            std::cout << std::endl;
        }
        for (int i = 0; i < (size*3) + 2; i++) {
            std::cout << "-";
        }
        std::cout << std::endl;
    }

    void movement() { //moves and other checks
        auto c = getch();
        if (c == '$') {
            adminMode();
            return;
        }
        //Movement
        if (c == 97 && x > 0 && map[(y*size) + x - 1] != 0 && map[(y*size) + x - 1] != 5 && map[(y*size) + x - 1] != 4) {x--;} //a,placement check
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] != 0 && map[(y*size) + x + 1] != 5 && map[(y*size) + x + 1] != 4) {x++;} //d,placement check
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] != 0 && map[((y+1)*size) + x] != 5 && map[((y+1)*size) + x] != 4) {y++;} //s,placement check
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] != 0 && map[((y-1)*size) + x] != 5 && map[((y-1)*size) + x] != 4) {y--;} //w,placement check

        else if (c == 49) {breakBlock();} //1, break block check
        else if (c == 50) {attackLocation();} //2, attack location check
        else if (c == 51) {throw Save();} // 3, save

        if (map[((y)*size) + x] == 2) {LuckyBlock(); map[((y)*size) + x] = 1;} // ? check
        if (map[((y)*size) + x] == 3) {throw Win();} // $ check
        if (map[((y)*size) + x] == 4) {attackLocation();} // Enemy check
        if (score < 0) {score = 0;} // no negative score
        if (isEnemyAlive) {enemyAI();} //movement of the enemy
    }

    void breakBlock() { //breaks a block
        auto c = getch();
        if (c == 97 && x > 0 && map[(y*size) + x - 1] == 0) {map[(y*size) + x - 1] = 1; score += blockScore;} //a, placement check & rock check
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] == 0) {map[(y*size) + x + 1] = 1; score += blockScore;} //d, placement check & rock check
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] == 0) {map[((y+1)*size) + x] = 1; score += blockScore;} //s, placement check & rock check
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] == 0) {map[((y-1)*size) + x] = 1; score += blockScore;} //w, placement check & rock check
    }

    void attackLocation() { // Attack Location if map = 4
        auto c = getch();
        if (!isEnemyAlive) {return;}
        Enemy *enemy = new Enemy();
        if (c == 97 && x > 0 && map[(y*size) + x - 1] == 4) { //a
            map[(y*size) + x - 1] = 1; 
            health = enemy->fight(health);
            score += (std::rand() % attackScore) + attackScore;
            isEnemyAlive = false;
        } 
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] == 4) {//d
            map[(y*size) + x + 1] = 1; 
            health = enemy->fight(health);
            score += (std::rand() % attackScore) + attackScore;
            isEnemyAlive = false;
        } 
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] == 4) {//s
            map[((y+1)*size) + x] = 1;
            health = enemy->fight(health);
            score += (std::rand() % attackScore) + attackScore;
            isEnemyAlive = false;
        }
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] == 4) {//w
            map[((y-1)*size) + x] = 1;
            health = enemy->fight(health);
            score += (std::rand() % attackScore) + attackScore;
            isEnemyAlive = false;
        }
    }

    void blankAction() { //Skeleton action function
        auto c = getch();
        if (c == 97 && x > 0) {} //a, placement check
        else if (c == 100 && x < size - 1) {} //d, placement check
        else if (c == 115 && y < size - 1) {} //s, placement check
        else if (c == 119 && y > 0) {} //w, placement check
    }

    void LuckyBlock() { //runs lucky block animation and affects player stats here
        clear();
        std::string luck = LuckyBlockFunction(memory.testLuck());
        if (luck == "HP") {modifyHealth((std::rand() % luckyHealth)+1, "increase");}
        if (luck == "SCORE") {score += std::rand() % luckyScore;}
        if (luck == "BONUS") {score += std::rand() % luckyScore; modifyHealth((std::rand() % luckyHealth)+1, "increase");}
        if (luck == "-SCORE") {score -= std::rand() % (luckyScore/2);}
        if (luck == "-HP") {modifyHealth((std::rand() % (luckyHealth-1))+1, "decrease");}
        if (score < 0) {score = 0;} // no negative score
    }

    int* generate(int size) { //generates new map with specific size
        int *map = new int[size * size];
        std::srand(std::time(nullptr));
        for (int i = 0; i < (size*size); i++) { //creates blocks, spaces, enemies
            int val = std::rand() % 13;
            if (val == 10 || val == 11) {val = 5;}
            else if (val > 0) {val = 1;}
            map[i] = val;
        }
        for (int i = 0; i < size - 6; i++) { //randomly places ?
            int pos = std::rand() % (size*size);
            while (map[pos] != 1) {
                pos = std::rand() % (size*size);
            }
            map[pos] = 2;
        }
         //randomly places {*} enemy
        int pos = std::rand() % (size*size);
        while (map[pos] != 1 && map[pos] != 2) {
            pos = std::rand() % (size*size);
        }
        map[pos] = 4;
        isEnemyAlive = true;
        enemyLocation = pos;
        map[0] = 1; //clear start
        map[((std::rand() % (size/2)) * size) + (2*size)] = 3; //$
        return map;
    }

    void newLevel() { //starts next level
        size++;
        level++;
        map = generate(size);
        setPosition(0,0);
    }

    void loadGame() { //loads game file into memory
        file.destruct(&memory);
        if (!memory.readEndBit()) { // end bit 0 (no cont) == true
            std::cout << std::endl << "Invalid Game File. Starting new Game." << std::endl;
            newGame();
            return;
        }
        MemoryChunk* temp = memory.start->next->next;
        level = temp->asInteger();
        temp = temp->next;
        score = temp->asInteger();
        while(temp->next != nullptr && temp->asInteger() < 64) {
            temp = temp->next;
            char charTemp = temp->asInteger();
            if (charTemp < 10) {charTemp += 48;}
            else if (charTemp < 36) {charTemp += 65 - 10;}
            else if (charTemp < 63) {charTemp += 97 - 36;}
            else {charTemp = 42;}
            name.push_back(charTemp);
        }
        size = (level * 2) + 6;
        map = generate(size);
        memory.start->next->next = nullptr;
    }
    void newGame() { //starts new gamefile
        memory = Memory();
        memory.memorySet();
    }
    void constructFile() { //constructs game file from memory
        memory.newChunk();
        memory.addInteger(level, intCapacity); //level
        memory.addInteger(score, intCapacity); //newscore
        for (int i = 0; i < name.length(); i++) { 
            int number;
            if (int(name[i]) >= 48 && int(name[i]) <= 57) { //0-9 (10)
                number = int(name[i]) - 48;
            }
            else if (int(name[i]) >= 65 && int(name[i]) <= 90) {// A-Z (26)
                number = int(name[i]) - 65 + 10;
            }
            else if (int(name[i]) >= 97 && int(name[i]) <= 122) { //a-z (26)
                number = int(name[i]) - 97 + 36;
            }
            else if (int(name[i]) != 10 & int(name[i]) != 32) { // * invalid (1) ... = 63 chars
                number = 63;
            }
            if (int(name[i]) != 10 & int(name[i]) != 32) {memory.addInteger(number, charCapacity);}
        }
        MemoryChunk* temp = memory.start;
        while (temp->next->next->next != nullptr) {temp = temp->next;}
        temp->next->next = nullptr;
        file.construct(&memory);
    }

    void setScore(int val) {score = val;} //setters for testing
    void setLevel(int val) {level = val;}
    
    void modifyHealth(int val, std::string condition = "decrease") { //modifies health
        if (condition != "decrease") {health += val; return;}
        else {
            health -= val;
            if (health < 0) {throw Loss();}
        }
    }

    void enemyAI() { //random movement of enemy if possible
        int check = std::rand() % 4;
        for (int i = 0; i < 4; i++) {
            //side checks
            if (checkLocation[check] == "left" && enemyLocation == 0) {continue;}
            //player checks
            if (enemyLocation - 1 == (y*size) + x  
                || enemyLocation + 1 == (y*size) + x 
                || enemyLocation - size == (y*size) + x 
                || enemyLocation + size == (y*size) + x ) {
                    Enemy *enemy = new Enemy();
                    health = enemy->fight(health);
                    map[enemyLocation] = 1; 
                    score += (std::rand() % attackScore) + attackScore;
                    isEnemyAlive = false;
                    return;
                }
            //movement checks
            if (checkLocation[check] == "left" && map[enemyLocation - 1] == 1 && enemyLocation % size != 0) {
                map[enemyLocation] = 1;
                enemyLocation = enemyLocation - 1;
                map[enemyLocation] = 4;
                return;
            }
            if (checkLocation[check] == "right" && map[enemyLocation + 1] == 1 && enemyLocation % size != size - 1){
                map[enemyLocation] = 1;
                enemyLocation = enemyLocation + 1;
                map[enemyLocation] = 4;
                return;
            }
            if (checkLocation[check] == "up" && map[enemyLocation - size] == 1) {
                map[enemyLocation] = 1;
                enemyLocation = enemyLocation - size;
                map[enemyLocation] = 4;
                return;
            }
            if (checkLocation[check] == "down" && map[enemyLocation + size] == 1){
                map[enemyLocation] = 1;
                enemyLocation = enemyLocation + size;
                map[enemyLocation] = 4;
                return;
            }
            check = (check + 1) % 4;
        }
    }

    /*
    This is an admin related function that when active will allow for increased customizations to the game.
    */

    void adminMode() {
        //if (adminActive) {return;}
        char adminConsole = ' ';
        int val;
        std::cout << "(type h for help)" << std::endl;
        while (adminConsole != 'x') {
            std::cout << std::endl << "$";
            adminConsole = getch();
            std::cout << adminConsole;
            if (adminConsole == 'h') {
                std::cout << std::endl << "Admin Help" << std::endl;
                std::cout << "h: help menu" << std::endl;
                std::cout << "!: sudo options" << std::endl;
                std::cout << "      ->a(direction: l,r,u,d): enemyAI movement" << std::endl;
                std::cout << "      ->k: kill enemy" << std::endl;
                std::cout << "      ->l: lucky block" << std::endl;
                std::cout << "      ->n: new level" << std::endl;
                std::cout << "m: modify stats" << std::endl;
                std::cout << "      ->h(val): add health" << std::endl;
                std::cout << "      ->l(val): set level" << std::endl;
                std::cout << "      ->s(val): set score" << std::endl;
                std::cout << "n: new game" << std::endl;
            }
            if (adminConsole == '!') {
                adminConsole = getch();
                std::cout << adminConsole;
                if (adminConsole == 'a') {
                    adminConsole = getch();
                    std::cout << adminConsole;
                    if (adminConsole == 'l') {for(int i = 0; i < 4; i++) {checkLocation[i] = "left";}}
                    if (adminConsole == 'r') {for(int i = 0; i < 4; i++) {checkLocation[i] = "right";}}
                    if (adminConsole == 'u') {for(int i = 0; i < 4; i++) {checkLocation[i] = "up";}}
                    if (adminConsole == 'd') {for(int i = 0; i < 4; i++) {checkLocation[i] = "down";}}
                }
                if (adminConsole == 'k') {isEnemyAlive = false;}
                if (adminConsole == 'l') {LuckyBlock();}
                if (adminConsole == 'n') {newLevel();}
            }
            if (adminConsole == 'm') {
                adminConsole = getch();
                std::cout << adminConsole;
                std::cin >> val;
                std::cout << val;
                if (adminConsole == 'h') {modifyHealth(val, "increase");}
                if (adminConsole == 'l') {setLevel(val - 1); newLevel();}
                if (adminConsole == 's') {setScore(val);}
            }
            if (adminConsole == 'n') {newGame(); adminConsole = 'x';}
        }
    }
};
