// ARBC - Assembly but Readable and Better (Compiler)
// Author: Benjamin Widman

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Receive command line arguments
    std::string input_name = argv[1];
    std::string output_name = argv[2];

    // Open source code file
    std::ifstream input;
    input.open(input_name);
    if (!input.is_open()) {
        std::cout << "File not found!" << std::endl;
        return -1;
    }

    // Create file for binary output
    std::ofstream output(output_name);

    //
    // Compile source code
    //
    

    // Close files
    input.close();
    output.close();

    return 0;
}