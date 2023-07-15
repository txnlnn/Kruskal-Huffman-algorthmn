#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// random seed number :1211301744
void generateHuffmanInputFile(string filename, int words)
{

    srand(1211301744);

    ofstream outfile(filename);

    int numUniqueChars = rand()% 26; // generate num of unique char, only have max 26 char

    outfile << numUniqueChars << endl; 

    vector<char> characters; 
    for (char c = 'A'; c <= 'Z'; c++) {
        characters.push_back(c);
    }

    random_shuffle(characters.begin(), characters.end()); //shuffle the char
    sort(characters.begin(), characters.begin() + numUniqueChars);

    for (int i = 0; i < numUniqueChars; i++) { //limit with numUniqueChars
        outfile << characters[i] << endl;
    }

   
    
    // generate number of words with spaces
    for (int i = 0; i < words; i++) {
        int wordLength = rand() % 10 + 1; // Generate random word length between 1 and 10
        string word;
        for (int j = 0; j < wordLength; j++) {
            char c = characters[rand() % numUniqueChars]; // Randomly select a character from the specified list
            word += c;
        }
        outfile << word << " ";
    }

    outfile.close();
}

int main()
{

    generateHuffmanInputFile("huffmancoding_00000010_input.txt", 10);
    generateHuffmanInputFile("huffmancoding_00000100_input.txt", 100);
    generateHuffmanInputFile("huffmancoding_00001000_input.txt", 1000);
    generateHuffmanInputFile("huffmancoding_00010000_input.txt", 10000);
    generateHuffmanInputFile("huffmancoding_00100000_input.txt", 100000);
    return 0;
}
