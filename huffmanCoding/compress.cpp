#include "HCTree.hpp"
#include <cstring>
using namespace std;

int main (int argc, char* argv[]) {
    // Error if user runs program incorreclty
    if (argc != 3) {
        error("Error: Files must be formatted as such: './compress <original file> <compressed_file>!");
        return -1;
    }

    // Error is user uses same file for input and output
    if (strcmp(argv[1], argv[2]) == 0) {
        error("Error: Both input and output files must be unique!");
        return -1;
    }

    // Open the input file for reading
    FancyInputStream inFile(argv[1]);

    if (inFile.filesize() <= 0) {
         FancyOutputStream outFile(argv[2]);
        return 0;
    }

    // read bytes from the file
    vector<int> freqs(256, 0);
    for (int i = 0; i < inFile.filesize(); i++) {
        if (!inFile.good()) {
            error("Error: stream's error state flags are set!");
            return -1;
        }
        // counting the number of occurrences of each byte value
        unsigned char input = inFile.read<char>();
        freqs[input]++;
    }
    // use byte count to construct a Huffman coding tree
    HCTree tree;
    tree.build(freqs);

    // open the output file for writing
    FancyOutputStream outFile(argv[2]);
    // write enough info ("file header") to the ouput file
    for (unsigned long int i = 0; i < freqs.size(); i++) {
        outFile.write<int>(freqs[i]);
    }
    // move back to the beginning of input file
    inFile.reset();

    // using the Huffman coding tree, encode input file and append as a code of bits to the output file after the header
    for (int i = 0; i < inFile.filesize(); i++) {
        tree.encode(inFile.read<char>(), outFile);
    }

    // close the input and output files
    inFile.reset();
    outFile.flush();

    return 0;
}
