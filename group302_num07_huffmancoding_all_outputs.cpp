#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>
#include<fstream>
#include<algorithm>
#include<cmath>
#include <chrono>
#include <iomanip>
using namespace std;

#define EMPTY_STRING ""

// A Tree node
struct Node
{
    char ch;
    int freq;
    Node *left, *right;
};

Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct compare
{
    bool operator()(const Node* l, const Node* r) const
    {
        // the highest priority item has the lowest frequency
        return l->freq > r->freq;
    }
};

bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

void encode(Node* root, string str, map<char, string> &huffmanCode)
{
    if (root == nullptr) {
        return;
    }
    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void HuffmanCode(string text, int charCount, string sLine)
{
    auto start = chrono::system_clock::now();

    if (text == EMPTY_STRING) {
        return;
    }

    unordered_map<char, int> frequency;
    for (char ch: text) {
        frequency[ch]++;
    }

    priority_queue<Node*, vector<Node*>, compare> priorityQueue;

    for (auto pair: frequency) {
        priorityQueue.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (priorityQueue.size() != 1)
    {

        Node* left = priorityQueue.top();
        priorityQueue.pop();
        Node* right = priorityQueue.top();
        priorityQueue.pop();
        int sum = left->freq + right->freq;
        priorityQueue.push(getNode('\0', sum, left, right));
    }

    Node* root = priorityQueue.top();
    map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    // open the new file output the data to the file
    //string filename = "huffmancoding_00000010_output.txt ";
    //string filename = "huffmancoding_00000100_output.txt ";
    //string filename = "huffmancoding_00001000_output.txt ";
    //string filename = "huffmancoding_00010000_output.txt ";
    string filename = "huffmancoding_00100000_output.txt ";

    
    ofstream outFile;
    outFile.open(filename);

    outFile << sLine << endl;

    double totalSize = 0;
    for (auto pair: huffmanCode) {
            for(auto f: frequency){
                if(f.first == pair.first){
                    int CharBitSize = (f.second) * (pair.second).size();
                    outFile << pair.first << "  " << f.second << "  " << pair.second << "  " << CharBitSize << endl;
                    totalSize += CharBitSize;
                }
            }
    }

    double totalTextBit = charCount * 7;

    double totalSpace = (totalSize * 100) / totalTextBit;

    outFile << totalSize << " -bit out of " << totalTextBit << " -bit" << endl;
    outFile <<"total space "<< fixed << setprecision(2) << totalSpace << " %" << endl;

    auto end = chrono::system_clock::now();

    chrono::duration<double> duration = end - start;

    outFile << defaultfloat << duration.count() << "s" << endl;

}

int main()
{

    ifstream readFile;

    //Read the name of input file
    //string readfilename = "huffmancoding_00000010_input.txt";
    //string readfilename = "huffmancoding_00000100_input.txt";
    //string readfilename = "huffmancoding_00001000_input.txt";
    //string readfilename = "huffmancoding_00010000_input.txt";
    string readfilename = "huffmancoding_00100000_input.txt";
    

    readFile.open(readfilename);


    if (! readFile) {
        cout << "Error opening input file." << endl;
        return 1;
    }
    string sLine;
    if (readFile.good()){
        getline(readFile, sLine);
    }

    string text;
    int charCount;

    while (getline(readFile, text)) {
            getline(readFile, text);
            text.erase(remove(text.begin(), text.end(), ' '), text.end());
            charCount = text.length();
    }

    HuffmanCode(text, charCount, sLine);
    cout << "The output file is completed."<<endl;

    return 0;
}
