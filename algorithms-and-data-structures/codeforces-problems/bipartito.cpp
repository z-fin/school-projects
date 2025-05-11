/*
/
/   Igna y Martín continúan sus aventuras! Como todos en TDA saben, a los JTPs les gustan los grafos bipartitos, especialmente los árboles.
/   Un grafo bipartito es un grafo cuyos vértices se pueden dividir en 2 conjuntos de tal manera que para cada arista (u,v) que pertenece al grafo, u y v
/   pertenecen a diferentes conjuntos. Podés encontrar una definición más formal de un grafo bipartito en la sección de notas a continuación.
/   Los JTPs le dieron a Igna y Martín un árbol que consiste en n nodos y les pidieron que agregaran aristas al mismo de tal manera que el grafo siga siendo bipartito.
/   Además, después de agregar estas aristas, el grafo debe ser simple (no debe contener loops, ni aristas múltiples). ¿Cuál es el número máximo de aristas que pueden agregar?
/   Un loop es una arista que conecta a un nodo consigo mismo. Un grafo no contiene aristas múltiples cuando para cada par de nodos no hay más de una arista entre ellos.
/   Un ciclo y un loop no son lo mismo.
/
/   Submission: https://codeforces.com/gym/551343/submission/287135292
/
*/


#include <iostream>
#include <vector>
 
#define RED 0
 
using namespace std;
 
long long int reds = 0;
long long int blues = 0;
 
void dfs(int v, vector<vector<int>> &adjacencyList, int colorFather, vector<bool> &visited)
{
    visited[v] = true;
 
    int team = 1 - colorFather;
 
    // cout << "vertice " << v << " es team: " << team << endl;
 
    if (team == RED)
    {
        reds++;
    }   
    else
    {
        blues++;
    }
 
    for (int i = 0; i < adjacencyList[v].size(); i++)
    {
        int neighbor = adjacencyList[v][i];
        if (!visited[neighbor])
        {
            dfs(neighbor, adjacencyList, team, visited);
        }
    }
}
 
int main()
{
    int n;
    cin >> n;
 
    vector<vector<int>> adjacencyList(n + 1);
 
    int m = n - 1;
 
    while (m > 0)
    {
        int v, u;
        cin >> v >> u;
 
        adjacencyList[v].push_back(u);
        adjacencyList[u].push_back(v);
 
        m--;
    }
 
    vector<bool> visited(n + 1, false);
 
    dfs(1, adjacencyList, RED, visited);
 
    long long int maxPossibleEdges = reds * blues;
 
    cout << maxPossibleEdges - n + 1 << endl;
 
    return 0;
}