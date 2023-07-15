#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

int totalWeight = 0;

struct Edge
{
    int src, dest, weight; // source & destination
};


/*
Parent is an array that holds the parent of each element in the set. The initial values are set to -1.
This recursive function ensures that all elements in a set ultimately points to the representative element.
*/
int find(int parent[], int i)
{
    if (parent[i] == -1)
        return i;
    return find(parent, parent[i]);
}

/*
The Union function updates the parent array to indicate that the two vertices belong
to the same set.
*/
void Union(int parent[], int x, int y)
{
    int xset = find(parent, x);
    int yset = find(parent, y);
    parent[xset] = yset;
}

void kruskalMST(vector<Edge> edges, int V, const vector<string>& vertex_pair, ofstream& outputFile)
{
    int* parent = new int[V];
    fill_n(parent, V, -1); // fill the parent array with -1

    // An empty vector of Edge objects to store the edges of the minimum spanning tree
    vector<Edge> result;
    int e = 0; // number of edges

    while (e < V-1) // V-1 is the number of edges in a MST
    {
        int minWeight = 101; // max weight in our input file is 100
        Edge minEdge; // to store edge with minimum weight

        // Find the minimum weight edge in edges vector
        for (const auto& edge: edges)
        {
            int x = find(parent, edge.src);
            int y = find(parent, edge.dest);

            // if the parent of src and parent of dest are different (will not create a cycle)
            if (x != y && edge.weight < minWeight)
            {
                minWeight = edge.weight;
                minEdge = edge;
            }
        }

        // Add the minimum weight edge to the result vector
        result.push_back(minEdge);
        Union(parent, minEdge.src, minEdge.dest);
        e++;
    }

    for (const auto& edge : result)
    {
        outputFile << vertex_pair[edge.src] << " " << vertex_pair[edge.dest] << " " << edge.weight << endl;
        totalWeight += edge.weight;
    }

    outputFile << totalWeight << endl;

    delete[] parent;
}

int main()
{
    // 10 vertices
    ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00000010_input.txt");
    ofstream outputFile("kruskalwithoutpq_am_00000010_output.txt");

    // 100 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00000100_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00000100_output.txt");

    // 1000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00001000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00001000_output.txt");

    // 2000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00002000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00002000_output.txt");

    // 5000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00005000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00005000_output.txt");

    // 10000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00010000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00010000_output.txt");

    // 50000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00050000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00050000_output.txt");

    // 100000 vertices
    //ifstream infile("kruskalwithoutpq_kruskalwithpq_am_00100000_input.txt");
    //ofstream outputFile("kruskalwithoutpq_am_00100000_output.txt");

    auto start = chrono::system_clock::now();
    int V; // number of vertices
    infile >> V;
    outputFile << V << endl;

    vector<string> vertex_pair(V);
    for (int i = 0; i < V; i++)
    {
        int vertex_index;
        string vertex_name;
        infile >> vertex_index >> vertex_name;
        vertex_pair[vertex_index] = vertex_name;
    }

    for (int i = 0; i < V; i++)
    {
        outputFile << i << " " << vertex_pair[i] << endl;
    }

    int** matrix = new int*[V];
    for (int i = 0; i < V; i++) {
        matrix[i] = new int[V];
    }

    string value;
    for (int x = 0; x < V; x++)
    {
        for (int y = 0; y < V; y++)
        {
            infile >> value;
            if (value == "i")
                matrix[x][y] = 0;
            else
                matrix[x][y] = stoi(value);
        }
    }

    vector<Edge> edges;
    for (int x = 0; x < V; x++)
    {
        for (int y = x + 1; y < V; y++)
        {
            if (matrix[x][y] != 0)
            {
                Edge e;
                e.src = x;
                e.dest = y;
                e.weight = matrix[x][y];
                edges.push_back(e);
            }
        }
    }

    if (outputFile.is_open())
    {
        kruskalMST(edges, V, vertex_pair, outputFile);
        auto end = chrono::system_clock::now();
        chrono::duration<double> duration = end - start;
        outputFile << duration.count() << "s\n";
        outputFile.close();
    }
    else
    {
        cout << "Unable to open output file." << endl;
    }

    // Free the dynamically allocated memory
    for (int i = 0; i < V; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
