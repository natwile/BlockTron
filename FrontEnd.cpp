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
public: 
    Memory memory;
    FrontEnd(int s){  //game creation
        x = 0;
        y = 0;
        size = s;
        map = generate(size);
        score = 0;
        level = 1;
        file = File();
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
                
            }
            std::cout << "|";
            if (i == 2) {std::cout << "    Level: " << level;}
            if (i == 3) {std::cout << "    Score: " << score;}
            if (i == 4) {std::cout << "    1->(w,a,s,d) : Break Block";}
            if (i == 6) {std::cout << "    3 : Save Game";}
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
        for (int i = 0; i < (size*size); i++) {
            int val = std::rand() % 4;
            if (val > 0) {val = 1;}
            map[i] = val;
        }
        for (int i = 0; i < 2; i++) {
            int pos = std::rand() % (size*size);
            while (map[pos] != 1) {
                pos = std::rand() % (size*size);
            }
            map[pos] = 2;
        }
        map[0] = 1;
        map[(std::rand() % size) * size] = 3;
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
        level = memory.start->next->next->asInteger();
        score = memory.start->next->next->next->asInteger();
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
        memory.addInteger(level);
        memory.addInteger(score);
        file.construct(&memory);
    }

    void setScore(int val) {score = val;}
    void setLevel(int val) {level = val;}
};
