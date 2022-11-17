// ASMPPC - Assembly++ Compiler
// Author: Benjamin Widman

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Split string with space as separator
std::vector<std::string>& split(const std::string& s) {
    static auto split_s = std::vector<std::string>();
    size_t index1 = 0, index2 = 0; // Start of word and end of word

    while ((index2 = s.find_first_of(' ', index1)) != std::string::npos) {
        std::string element = s.substr(index1, index2 - index1);
        if (element != "") // Don't add empty elements
            split_s.emplace_back(element);
        index1 = index2 + 1;
    }
    return split_s;
}

int main(int argc, char* argv[]) {
    // Receive command line arguments
    if (argc < 2) {
        std::cout << "Syntax: asmppc source.asmpp output.asmppbin" << std::endl;
        return 1;
    }
    const std::string input_name = argv[1];
    const std::string output_name = argv[2];

    // Open source code file
    std::ifstream input(input_name);
    if (!input.is_open()) {
        std::cout << "File not found!" << std::endl;
        return 1;
    }
    // Create file for binary output
    std::ofstream output(output_name);

    //
    // Compile source code
    //
    std::string line;
    // Read file until there are no more lines (EOF)
    while (std::getline(input, line)) {
        auto arguments = split(line);
        char bin_instruction;

        if (arguments[0][0] == '#') {
            // One of instruction 1-5
            std::string instruction = arguments[1];
            std::string hl = arguments[2]; // Crash
            if (instruction == "=" && arguments[2].length() == 1) { // Assign but not register on right hand side
                // Only RI-type
                char opcode = 0b100 << 5;
                char reg = (int(arguments[0][1]) - 1) << 3;
                char imm = std::stoi(arguments[2]);
                
                bin_instruction = opcode + reg + imm;
            } else {
                // 2R-type
                char opcode;
                char reg1 = (int(arguments[0][1]) - 1) << 3;
                char reg2 = (int(arguments[2][1]) - 1) << 1;

                if (instruction == "+") {
                    opcode = 0b000 << 5;
                } else if (instruction == "==") {
                    opcode = 0b001 << 5;
                } else if (instruction == "<") {
                    opcode = 0b010 << 5;
                } else if (instruction == "=") {
                    opcode = 0b011 << 5;
                }
                bin_instruction = opcode + reg1 + reg2;
            }
        } else {
            // One of instruction 6-8
            std::string instruction = arguments[0];

            if (instruction == "if") {
                // Only RI-type
                char opcode = 0b111 << 5;
                char reg = (int(arguments[2][1]) - 1) << 3;
                char imm = std::stoi(arguments[3]);

                bin_instruction = opcode + reg + imm;
            } else {
                // I-type
                char opcode;
                char imm = std::stoi(arguments[1]);

                if (instruction == "syscall") {
                    opcode = 0b101 << 5;
                } else /*jump*/ {
                    opcode = 0b110 << 5;
                }
                bin_instruction = opcode + imm;
            }
        }
        std::cout << "Instruction: ";
        std::cout << std::ios::binary << bin_instruction << std::endl;
        // Output the instruction byte to file
        output.write(&bin_instruction, 1);
    }

    // Close files
    input.close();
    output.close();

    return 0;
}