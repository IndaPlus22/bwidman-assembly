// Assembly++ Virtual Machine
// Author: Benjamin Widman

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

typedef unsigned char byte;

int main(int argc, char* argv[]) {
    // Check if there are sufficient command line arguments
    if (argc < 2) {
        std::cout << "No file provided as argument" << std::endl;
        return 1;
    }

    // Open file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "File not found!" << std::endl;
        return 1;
    }
    // Check for debug mode
    bool debug = argc >= 3 && argv[2] == "-debug";
    int delay = debug ? std::stoi(argv[3]) : 0;

    //
    // Run executable
    //
    char registers[4] = {};
    
    byte byte_instruction;
    while (file.read((char*)&byte_instruction, 1)) {
        // Debugging tools
        if (debug) {
            Sleep(delay);
            system("clear");
            std::cout << "#1: " << registers[0] <<
                        "\n#2: " << registers[1] <<
                        "\n#3: " << registers[2] <<
                        "\n#4: " << registers[3] << std::endl;
        }

        byte opcode = byte_instruction >> 5;
        byte a = (byte_instruction & 0b11000) >> 3;
        byte b = (byte_instruction & 0b110) >> 1;

        char immRI = byte_instruction & 0b11; // Remove sign
        if ((byte_instruction & 0b100) != 0) // Signed
            immRI = -immRI;

        char immI = byte_instruction & 0b1111; // Remove sign
        if ((byte_instruction & 0b10000) != 0) // Signed
            immI = -immI;

        switch (opcode) {
            // Add (2R)
            case 0b000: registers[a] += registers[b]; break;
            // Set equal to (2R)
            case 0b001: registers[a] = registers[a] == registers[b]; break;
            // Set less than (2R)
            case 0b010: registers[a] = registers[a] < registers[b]; break;
            // Move (2R)
            case 0b011: registers[a] = registers[b]; break;
            // Assign value (RI)
            case 0b100: registers[a] = immRI; break;
            // System call (I)
            case 0b101:
                switch (immI) {
                    // Print
                    case 0: std::cout << registers[0] << std::endl; break;
                    // Read
                    case 1: std::cin >> registers[0]; break;
                }
                break;
            // Jump (I)
            case 0b110: file.seekg(immI + 1, std::ios::cur); break;
            // Jump if true (RI)
            case 0b111:
                if (registers[a])
                    file.seekg(immRI + 1, std::ios::cur);
                break;
        }
    }

    return 0;
}