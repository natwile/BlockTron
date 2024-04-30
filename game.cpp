#include "FrontEnd.cpp"

int main() {
    FrontEnd f = FrontEnd(8); 
    char user = 0;
    std::cout << "Welcome to Block-Tron!" << std::endl;
    std::cout << "Press 1 for new game" << std::endl;
    std::cout << "Press 2 for load game" << std::endl;
    std::cin >> user;
    if (user == 50) {f.loadGame();}
    else {f.newGame();}
    while (true) {
        try {
            while(true) {
                f.print();
                f.movement();
                f.clear();
            }
        }
        catch (Win&) {
            f.newLevel(); 
        }
        catch (Loss&) {
            f.memory.endBit();
            std::cout << std::endl << "You lost." << std::endl;
            return 0;
        }
        catch (Save&) {
            f.constructFile();
            std::cout << std::endl << "You Saved." << std::endl;
            return 0;
        }
    }
    return 0;
}