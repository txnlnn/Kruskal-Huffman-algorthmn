#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

//this structures the edge of what it is and what it has
struct Edge {
    int src, dest, weight;
};

//this puts the inputfile vertex and weights into a graph
struct Graph {
    int V;
    vector<string> vertexNames;
    vector<vector<int>> adjMatrix;
};

//this compares the weights of the edges
struct CompareEdges {
    bool operator()(const Edge& e1, const Edge& e2) {
        return e1.weight > e2.weight;
    }
};

//This is to split the string from position
string splitStringWithPosition(const string& str, const string& delimiter = " ") {
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }

    return str.substr(start, end);
}

//reads the input file and understands the format of the input file
Graph readInputFile(const string& filename) {
    ifstream inputFile(filename);
    Graph graph;

    if (inputFile.is_open()) {
        inputFile >> graph.V; //graphs the input file from the matrix
        int totalInputLines = graph.V * 2;
        inputFile.ignore(); // Ignore the newline character after reading V
        graph.vertexNames.resize(graph.V);

        for (int i = 0; i < graph.V; i++) {
            string line;
            getline(inputFile, line);
            string name = splitStringWithPosition(line, " "); //split the index and vertex names
            graph.vertexNames[i] = name;
        }

        graph.adjMatrix.resize(graph.V, vector<int>(graph.V)); //graph and resize the matrix in the input file
        for (int i = 0; i < graph.V; i++) {
            string line;
            getline(inputFile, line);
            istringstream iss(line);
            for (int j = 0; j < graph.V; j++) {
                string weight;
                iss >> weight;
                if (weight == "i") {
                    graph.adjMatrix[i][j] = 0; // Set infinity as 0
                } else {
                    graph.adjMatrix[i][j] = stoi(weight);
                }
            }
        }

        inputFile.close();
    } else {
        cout << "Failed to open the input file." << endl;
    }

    return graph;
}

//used to find parent nodes of the S.T
int find(vector<int>& parent, int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

//identify the roots
void unionSet(vector<int>& parent, vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

//kruskal algorithm with Priority Queue
vector<Edge> kruskalMST(int V, const vector<vector<int>>& graph) {
    vector<Edge> result;
    int e = 0;
    priority_queue<Edge, vector<Edge>, CompareEdges> pq; //this creates it with priority queue

    vector<int> parent(V);
    vector<int> rank(V, 0);
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    //this is the priority queue
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            if (graph[u][v] != 0) {
                pq.push({u, v, graph[u][v]});
            }
        }
    }

    //finds the next least edge in the immediate pq
    while (e < V - 1 && !pq.empty()) {
        Edge nextEdge = pq.top();
        pq.pop();

        int x = find(parent, nextEdge.src);
        int y = find(parent, nextEdge.dest);

        if (x != y) {
            result.push_back(nextEdge);
            unionSet(parent, rank, x, y);
            e++;
        }
    }

    return result;
}

//function for total weight calculation
int calculateTotalWeight(const vector<Edge>& edges) {
    int totalWeight = 0;
    for (const auto& edge : edges) {
        totalWeight += edge.weight;
    }
    return totalWeight;
}

//writes the output into text file with the desired format
void writeOutputFile(const string& filename, const Graph& graph, const vector<Edge>& edges, const vector<string>& vertexNames, int totalWeight, double duration){
    ofstream outputFile(filename);
    if(outputFile.is_open()){
        outputFile << vertexNames.size() << endl; //writes the number of vertices

        //this loop auto prints index as the size already gave the number of indexes so loop it
        for(int i = 0; i<vertexNames.size(); i++){
            outputFile << i << " " << vertexNames[i] << endl; //writes the vertex index alongside the vertex names in a loop
        }

        //loops the print for edge and weights
        for(const auto& edge: edges){
            outputFile << vertexNames[edge.src] << "\t-\t" << vertexNames[edge.dest] << "\t:" << setw(2) << edge.weight << endl;
        }

        outputFile << totalWeight << endl; //prints total weight from the MST
        outputFile << duration << "s" << endl; //prints the duration of the calculation
        outputFile.close();

        cout << "Output written to " << filename << "successfully." << endl;
    }else{
        cout << "Failed to open the output File." << endl;
    }
}

//function to easily run the functions as it is done in switch method for each number of inputs
void execution(int vertices, const string& inputFile, const string& outputFile){
    cout << "You selected " << vertices << " vertices." << endl;

    Graph graph = readInputFile(inputFile);
    auto start = chrono::system_clock::now();
    vector<Edge> mst = kruskalMST(graph.V, graph.adjMatrix);
    int totalWeight = calculateTotalWeight(mst);
    auto stop = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = stop-start;
    double duration = elapsed_seconds.count();

    //sorts the edge in weight order then sorts in alphabetical order from between the vertex pairs
    sort(mst.begin(), mst.end(), [](const Edge& e1, const Edge& e2){
        if(e1.weight != e2.weight)
            return e1.weight < e2.weight;
        if(e1.src != e2.src)
            return e1.src < e2.src;
        return e1.dest < e2.dest;
    });

    writeOutputFile(outputFile, graph, mst, graph.vertexNames, totalWeight, duration); //prints the MST in text file
    cout << "DONE Successfully" << endl;
    cout << "Time taken: " << duration << "s" << endl;
}

//used to accept enter as input
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//main
int main() {
    int choice;
    bool exitProgram = false;

    while(!exitProgram){
        cout << "Menu: " << endl;
        cout << "1. 10" << endl;
        cout << "2. 100" << endl;
        cout << "3. 1000" << endl;
        cout << "4. 2000" << endl;
        cout << "5. 5000" << endl;
        cout << "6. 10000" << endl;
        cout << "7. 50000" << endl;
        cout << "8. 100000" << endl;
        cout << "0. Exit Program" << endl;

        cout << "Enter your choice (0-8): ";
        if(!(cin>>choice)){
            clearInputBuffer();
            cout << "Invalid input. Please enter a number from 0-8. " << endl;
            continue;
        }

        //switch function to choose between the input files generated by group mate
        switch(choice){
            case 0:
                exitProgram = true;
                break;
            case 1:
                execution(10, "kruskalwithoutpq_kruskalwithpq_am_00000010_input.txt","kruskalwithpq_am_00000010_output.txt");
                break;
            case 2:
                execution(100, "kruskalwithoutpq_kruskalwithpq_am_00000100_input.txt","kruskalwithpq_am_00000100_output.txt");
                break;
            case 3:
                execution(1000, "kruskalwithoutpq_kruskalwithpq_am_00001000_input.txt","kruskalwithpq_am_00001000_output.txt");
                break;
            case 4:
                execution(2000, "kruskalwithoutpq_kruskalwithpq_am_00002000_input.txt","kruskalwithpq_am_00002000_output.txt");
                break;
            case 5:
                execution(5000, "kruskalwithoutpq_kruskalwithpq_am_00005000_input.txt","kruskalwithpq_am_00005000_output.txt");
                break;
            case 6:
                execution(10000, "kruskalwithoutpq_kruskalwithpq_am_00010000_input.txt","kruskalwithpq_am_00010000_output.txt");
                break;
            case 7:
                execution(50000, "kruskalwithoutpq_kruskalwithpq_am_00050000_input.txt","kruskalwithpq_am_00050000_output.txt");
                break;
            case 8:
                execution(100000, "kruskalwithoutpq_kruskalwithpq_am_00100000_input.txt","kruskalwithpq_am_00100000_output.txt");
                break;
            default:
                cout << "Invalid choice. Please enter a number from 0-8. " << endl;
                break;
        }

        clearInputBuffer();
        cout << "Press Enter to continue.";
        cin.ignore();
        cout << endl;
    }
    return 0;
}