#include <iostream>

void EnterRoom(int roomNumber) {
    std::cout << "Now entering Room " << roomNumber << "..." << std::endl;
}

int main() {
    int currentRoom = 1;
    while (true) {
        if (currentRoom == 1) {
            EnterRoom(1);
            currentRoom = 2;
        }
        else if (currentRoom == 2) {
            EnterRoom(2);
            currentRoom = 3;
        }
        else if (currentRoom == 3) {
            EnterRoom(3);
            currentRoom = 1; 
        }
    }
    return 0; 
}