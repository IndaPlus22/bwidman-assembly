// ASMPPC - Assembly++ Compiler
// Author: Benjamin Widman

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

std::vector<std::string>& split(std::string& s) {
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
    std::string input_name = argv[1];
    std::string output_name = argv[2];

    // Open source code file
    std::ifstream input(input_name);
    if (!input.is_open()) {
        std::cout << "File not found!" << std::endl;
        return -1;
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

        // One of instruction 1-5
        if (line[0] == '#') {

        } else {

        }
    }

    // Close files
    input.close();
    output.close();

    return 0;
}