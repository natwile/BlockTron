#include <conio.h>
#include "Memory.cpp"

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

/* FrontEnd class for visuals */
class FrontEnd {
private:
    int x;
    int y;
    int size;
    int *map;
    int score;
    int level;
    
    File file;
    int health;
public: 
    std::string name; 
    Memory memory;
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
        //Movement 
        if (c == 97 && x > 0 && map[(y*size) + x - 1] != 0) {x--;} //a,placement check
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] != 0) {x++;} //d,placement check
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] != 0) {y++;} //s,placement check
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] != 0) {y--;} //w,placement check

        else if (c == 49) {breakBlock();} //1, break block check
        else if (c == 50) {attackLocation();} //2, attack location check
        else if (c == 51) {throw Save();} // 3, save

        if (map[((y)*size) + x] == 2) {LuckyBlock(); map[((y)*size) + x] = 1;} // ? check
        if (map[((y)*size) + x] == 3) {throw Win();} // $ check
        if (score < 0) {score = 0;} // no negative score
    }

    void breakBlock() { //breaks a block
        auto c = getch();
        if (c == 97 && x > 0 && map[(y*size) + x - 1] == 0) {map[(y*size) + x - 1] = 1; score++;} //a, placement check & rock check
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] == 0) {map[(y*size) + x + 1] = 1; score++;} //d, placement check & rock check
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] == 0) {map[((y+1)*size) + x] = 1; score++;} //s, placement check & rock check
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] == 0) {map[((y-1)*size) + x] = 1; score++;} //w, placement check & rock check
    }

    void attackLocation() { // Attack Location if map = 4
        auto c = getch();
        if (c == 97 && x > 0 && map[(y*size) + x - 1] == 4) { //a
            map[(y*size) + x - 1] = 1; 
            score += 5;
            health -= 1;
        } 
        else if (c == 100 && x < size - 1 && map[(y*size) + x + 1] == 4) {//d
            map[(y*size) + x + 1] = 1; 
            score += 5;
            health -= 1;
        } 
        else if (c == 115 && y < size - 1 && map[((y+1)*size) + x] == 4) {//s
            score += 5;
            health -= 1;
        }
        else if (c == 119 && y > 0 && map[((y-1)*size) + x] == 4) {//w
            score += 5;
            health -= 1;
        }
    }

    void blankAction() { //Skeleton action function
        auto c = getch();
        if (c == 97 && x > 0) {} //a, placement check
        else if (c == 100 && x < size - 1) {} //d, placement check
        else if (c == 115 && y < size - 1) {} //s, placement check
        else if (c == 119 && y > 0) {} //w, placement check
    }

    void LuckyBlock() { //Tests luck till succession
        if(memory.testLuck()) {score += 5;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score += 15;}
        if(memory.testLuck()) {score -= 15;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score -= 20;}
        if(memory.testLuck()) {score += 5;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score += 15;}
        if(memory.testLuck()) {score -= 6;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score -= 20;}
        if(memory.testLuck()) {score -= 15;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score += 15;}
        if(memory.testLuck()) {score += 6;}
        if(memory.testLuck()) {score += 10;}
        if(memory.testLuck()) {score -= 6;}
    }

    int* generate(int size) { //generates new map with specific size
        int *map = new int[size * size];
        std::srand(std::time(nullptr));
        for (int i = 0; i < (size*size); i++) { //creates blocks or spaces
            int val = std::rand() % 6;
            if (val == 5) {val = 5;}
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
        memory.addInteger(level, intCapacity);
        memory.addInteger(score, intCapacity);
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

    void setScore(int val) {score = val;}
    void setLevel(int val) {level = val;}
    
    void modifyHealth(int val, std::string condition = "decrease") { //modifies health
        if (condition != "decrease") {health += val; return;}
        else {
            health -= val;
            if (health < 0) {throw Loss();}
        }
    }
};
