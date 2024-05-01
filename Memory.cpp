#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

/*  Design and Purpose  */
/*
-The design of this memory map is that of a doubly linked list.
    ->That is, a Linked List of Linked Lists that contain Nodes.
-This designed is specifically useful for maximum flexibility in a data structure.
-Each Node contains a boolean bit and a pointer to another NOde.
-Each Chunk(Linked List) contains a pointer to a head Node and another Chunk.
-Each Memory Contains a head(start) pointer to a Linked List.

-The purpose of the bool storage is to learn and test different kinds of knowledge.
    ->This allows for thinking of bits as storage (testing from Computer Systems Class).
    ->It also allows for constant manipulation of types and lengths of data stored.
-The purpose of the File class is to allow for easy transfer between file i/o and the Memory
-The purpose of the constants is to allow for changing data overflow limits and i/o file name.
-There are specific purposes to each helper function that do not directly affect the LL structure.
    ->asInteger() & addInteger() allow for transfer between bit patterns and unsigned integers.
    ->print() allow for easy visual testing of the data structures.
    ->testLuck(), memorySet(), endBit() are specifically used in the game sense.
-For the LL classes, there are typically getters,setters and constructors.
-There are also allocators allowing for easy and controlled memory allocation.
-Most of these classes are all public allowing access for the FrontEnd.
Created with V1.0
*/

/* Constants */
const int intCapacity = 2048; //12 bit score and level
const std::string fileName = "gamefile.txt"; //filename to input/output from

/* Single Memory Bit (Binary Node)*/
class MemoryBit {
private:
    bool bit;
public:
    MemoryBit* next;

    MemoryBit(bool val) { //constructs memory bit with specific val
        next = nullptr;
        bit = val;
    }
    bool getBit() { //returns the bit of the memory bit
        return bit;
    }
    void changeBit(bool val) { //changes bit of memory bit
        bit = val;
    }
};

/* Memory Chunk (Linked List of Bits)*/
class MemoryChunk {
public:
    MemoryBit* head;
    MemoryChunk* next;

    MemoryChunk() { //constructs empty memory chunk
        next = nullptr;
        head = nullptr;
    }

    void changeBit(int pos, bool val) { //changes bit to specific val at specific pos in memory chunk
        MemoryBit* temp = head;
        while (pos > 0) {
            temp = temp->next;
            pos--;
        }
        temp->changeBit(val);
    }
    bool getBit(int pos) { //returns bit at position within memory chunk
        MemoryBit* temp = head;
        while (pos > 0) {
            temp = temp->next;
            pos--;
        }
        return temp->getBit();
    }
    void addBit(bool val) { //adds bit to memory chunk
        if (head == nullptr) {
            head = new MemoryBit(val);
            return;
        }
        MemoryBit* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new MemoryBit(val);
    }

    int asInteger() { //returns memory chunk bits as an integer value (used for luck)
        int num = 0;
        int scale = 1;
        MemoryBit* temp = head;
        if (temp == nullptr) {return 0;}
        while (temp->next != nullptr) {
            scale *= 2;
            temp = temp->next;
        }
        temp = head;
        while (temp != nullptr) {
            num += (temp->getBit() * scale);
            scale /= 2;
            temp = temp->next;
        }
        return num;
    }
    void print() { //prints specific memory chunk
        MemoryBit* temp = head;
        while (temp != nullptr) {
            std::cout << temp->getBit();
            temp = temp->next;
        }
    }
};


/* Memory (Linked List of Binary Linked Lists)*/
class Memory {
public:
    MemoryChunk* start;

    Memory() { //constructs empty memory
        start = nullptr;
    }
    void print() { //prints all memory chunks
        MemoryChunk* temp = start;
        while (temp != nullptr) {
            temp->print();
            std::cout << " ";
            temp = temp->next;
        }
    }
    void addBit(bool val) { //adds bit to current memory chunk
        MemoryChunk* temp = start;
        if (start == nullptr) {
            start->addBit(val);
            return;
        }
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->addBit(val);
    }
    void newChunk() { //allocates new chunk within memory
        MemoryChunk* temp = start;
        if (temp == nullptr) {
            start = new MemoryChunk();
            return;
        }
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new MemoryChunk();
    }
    void memorySet() { //sets continuity bit and luck
        newChunk();
        addBit(1);
        newChunk();
        std::srand(std::time(nullptr));
        for (int i = 0; i < 4; i++) {
            int luck = std::rand() % 2;
            addBit(luck);
        }
    }
    void endBit() { //changes coninuity bit 
        start->changeBit(0,0);
    }
    bool testLuck() { //returns true/false for random luck event
        int condition = std::rand() % 3;
        condition++; //1-3
        int val = start->next->asInteger();
        if (condition == 1) {return val > 8;}
        if (condition == 2) {return val < 8;}
        if (condition == 3) {return std::rand() % 2;}
    }

    void addInteger(int val) { //adds an integer to memory with a specific capacity
        int scale = intCapacity;
        while (scale > 1) {
            if (val - scale >= 0) {
                val -= scale;
                addBit(1);
            }
            else {
                addBit(0);
            }
            scale /= 2;  
        }
        addBit(val == 1);
        newChunk();
    }
};

class File {
public:
    std::fstream data;
    char analyze;
    File() {}

    void construct(Memory* m) { //inserting memory into gamefile
        data.open(fileName, std::ios::out);
        MemoryChunk* temp1 = m->start;
        while(temp1 != nullptr) {
            MemoryBit* temp2 = temp1->head;
            while (temp2 != nullptr) {
                data << temp2->getBit();
                temp2 = temp2->next;
            }
            data << " ";
            temp1 = temp1->next;
        }
        data.close();
    }

    void destruct(Memory* m) { //reverting gamefile to memory
        data.open(fileName, std::ios::in);
        m->newChunk();
        while(!data.eof()) {
            analyze = data.get();
            if (analyze == 48 || analyze == 49) {
                m->addBit(int(analyze) - 48);
            }
            if (analyze == 10 || analyze == 32){
                m->newChunk();
            }
        }
        data.close();
    }
};
