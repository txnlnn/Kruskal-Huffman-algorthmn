#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

// random seed number :1211301744
void generateKruskalInputFile(string filename, int vertices)
{
    auto start = chrono::system_clock::now();

    srand(1211301744);

    ofstream outfile(filename);

    vector<vector<int>> adj_matrix(vertices, vector<int>(vertices, 0));

    outfile << vertices << endl; // num of vertices
    for (int i = 0; i < vertices; i++)
    {
        outfile << i << " "
                << "A" << i << endl; // vertex indexes and vertex names
    }

    // adjacency matrix graph
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (i == j)
            {
                adj_matrix[i][j] = 0; // no self-loops
            }
            else
            {
                int w = rand() % 101; // (limit it to 0-100)
                adj_matrix[i][j] = w;
                adj_matrix[j][i] = w; // since it's an undirected graph
            }
        }
    }

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (adj_matrix[i][j] == 0) // if equal 0 replace it with i
            {
                outfile << "i"
                        << " ";
            }
            else
                outfile << adj_matrix[i][j] << " ";
        }
        outfile << endl;
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Duration: " << duration.count() << "s\n";

    outfile.close();
}

int main()
{

    generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00000010_input.txt", 10);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00000100_input.txt", 100);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00001000_input.txt", 1000);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00002000_input.txt", 2000);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00005000_input.txt", 5000);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00010000_input.txt", 10000);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00050000_input.txt", 50000);
    //generateKruskalInputFile("kruskalwithoutpq_kruskalwithpq_am_00100000_input.txt", 100000);

    return 0;
}
