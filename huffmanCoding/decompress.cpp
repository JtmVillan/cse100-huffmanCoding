#include "HCTree.hpp"
#include <cstring>
using namespace std;

int main (int argc, char* argv[]) {
    // Error if user runs program incorreclty
    if (argc != 3) {
        error("Error: Files must be formatted as such: './compress <original file> <compressed_file>!");
        return -1;
    }

    // // Error is user uses same file for input and output
    if (strcmp(argv[1], argv[2]) == 0) {
        error("Error: Both input and output files must be unique!");
        return -1;
    }

    // open the input file for reading
    FancyInputStream inFile(argv[1]);

    if (inFile.filesize() == 0) {
        FancyOutputStream outFile(argv[2]);
        return 0;
    }

    // read the file header 
    vector<int> freqs(256, 0);
    int b;
    int total = 0;
    for (unsigned long int i = 0; i < freqs.size(); i++) {
        if (!inFile.good()) {
            error("Error: stream's error state flags are set!");
            return -1;
        }
        b = inFile.read<int>();
        freqs[i] = b;
        total += b;
    }
    // reconstruct the Huffman Coding tree
    HCTree tree;
    tree.build(freqs);

    // open the output file for writing 
    FancyOutputStream outFile(argv[2]);

    // using the Huffman coding tree, decode bits, write to output file
    for (int i = 0; i < total; i++) {
        outFile.write<char>(tree.decode(inFile));
    }

    // close the input and output file
    inFile.reset();
    outFile.flush();

    return 0;
}
