// ASMPPC - Assembly++ Compiler
// Author: Benjamin Widman

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char byte;

// Split string with space as separator
std::vector<std::string> split(std::string& s) {
    s += ' ';
    auto split_s = std::vector<std::string>();
    size_t index1 = 0, index2 = 0; // Start of word and end of word

    while ((index2 = s.find_first_of(' ', index1)) != std::string::npos) {
        std::string element = s.substr(index1, index2 - index1);
        if (element != "") // Don't add empty elements
            split_s.emplace_back(element);
        index1 = index2 + 1;
    }
    return split_s;
}

byte signed_imm(int number, int size) {
    byte imm = abs(number);
    if (number < 0) {
        byte sign = 0b1 << (size - 1);
        imm |= sign; // Set signed bit to 1
    }
    return imm;
}

void parse_line(std::string& line, std::ofstream& output) {
    std::vector<std::string> arguments = split(line);
    byte bin_instruction;

    if (arguments[0][0] == '#') {
        // One of instruction 1-5
        std::string instruction = arguments[1];
        
        if (arguments[2][0] != '#') { // Assign but not register on right hand side
            // RI-type
            byte opcode;
            byte a = (int(arguments[0][1]) - 48 - 1) << 3;
            byte imm = signed_imm(std::stoi(arguments[2]), 3);

            if (instruction == "=") {
                opcode = 0b100 << 5;
            } else { // +=
                opcode = 0b001 << 5;
            }
            
            bin_instruction = opcode + a + imm;
        } else {
            // 2R-type
            byte opcode;
            byte a = (int(arguments[0][1]) - 48 - 1) << 3;
            byte b = (int(arguments[2][1]) - 48 - 1) << 1;

            if (instruction == "+=") {
                opcode = 0b000 << 5;
            } else if (instruction == "<") {
                opcode = 0b010 << 5;
            } else if (instruction == "=") {
                opcode = 0b011 << 5;
            }
            bin_instruction = opcode + a + b;
        }
    } else {
        // One of instruction 6-8 or pseudo
        std::string instruction = arguments[0];

        if (instruction == "if") {
            // Pseudo instruction: if #a < #b jump imm
            std::string a = arguments[1];
            std::string b = arguments[3];
            int imm = std::stoi(arguments[5]);
            imm = imm > 0 ? imm + 1 : imm - 1;

            std::string less_than = a + " < " + b;
            std::string jumpif = "jumpif " + std::to_string(imm);
            parse_line(less_than, output);
            parse_line(jumpif, output);
            
            return;
        } else {
            // I-type
            byte opcode, imm;
            int number = std::stoi(arguments[1]);

            if (instruction == "syscall") {
                opcode = 0b101 << 5;
                imm = signed_imm(number, 5);
            } else if (instruction == "jump") {
                opcode = 0b110 << 5;
                // Cannot jump 0 rows so +1 away from 0 enables input of +-1-4
                number = number > 0 ? number - 1 : number + 1;
                imm = signed_imm(number, 5);
            } else if (instruction == "jumpif") {
                opcode = 0b111 << 5;
                // Cannot jump 0 rows so +1 away from 0 enables input of +-1-4
                number = number > 0 ? number - 1 : number + 1;
                imm = signed_imm(number, 5);
            }
            bin_instruction = opcode + imm;
        }
    }
    // Output the instruction byte to file
    output.write((char*)&bin_instruction, 1);
}

int main(int argc, char* argv[]) {
    // Check if there are sufficient command line arguments
    if (argc < 3) {
        std::cout << "Syntax: asmppc source.asmpp output.asmppbin" << std::endl;
        return 1;
    }
    // Open source code file
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cout << "File not found!" << std::endl;
        return 1;
    }
    // Create file for binary output
    std::ofstream output(argv[2]);
    
    //
    // Compile source code
    //
    std::string line;
    // Read file until there are no more lines (EOF)
    while (std::getline(input, line)) {
        if (line == "")
            continue;
        parse_line(line, output);
    }

    // Close files
    input.close();
    output.close();

    return 0;
}