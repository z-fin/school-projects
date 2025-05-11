/*
/   Tuki está ayudando al Gobierno de la Ciudad a instalar sistemas de seguridad en las distintas calles de la Capital.
/   Esta se modela como un multigrafo no dirigido con n nodos donde estos denotan esquinas de la ciudad mientras que los ejes representan las calles.
/   Para cada calle se conoce su longitud y el costo de colocar un sistema de seguridad en la misma, el cual es exactamente el doble de su longitud.
/   Como cubrir todas las calles es muy caro, el Gobierno se propone solo cubrir aquellas que pertenecen a algún camino mínimo entre el nodo 0 y el n−1, y está interesado en descubrir el costo del proyecto.
/   Debemos ayudar a Tuki a resolver este problema. Para eso, debemos diseñar un algoritmo que dada la descripción de la ciudad encuentre el costo de instalar sistemas de seguridad en todas las calles que pertenecen a algún camino mínimo entre el nodo 0 y el n−1.
/
/   Submission: https://codeforces.com/gym/562517/submission/292805393
/
*/
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

#define INF 1001

struct Edge {
    int v, u, w;
};

class Dijkstra {
private:
    int n, m;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adjList;
    vector<int> d_0; // distancias desde el nodo 0
    vector<int> d_n; // distancias desde el nodo n-1

    void initialize_single_source(int s, vector<int>& d) {
        d.assign(n, INF);
        d[s] = 0;
    }

    void relax(int u, int v, int w, priority_queue<pair<int,int>,vector<pair<int,int>>, greater<pair<int, int>>>& Q, vector<int>& d) {
        if (d[v] > d[u] + w) {
            d[v] = d[u] + w;
            Q.push({d[v], v}); // Lo agrego aca porque en cpp no hay una priority queue con valores dinamicos
        }
    }

public:
    Dijkstra(int _n, int _m, vector<Edge>& _edges, vector<vector<pair<int, int>>>& _adjList) : n(_n), m(_m), edges(_edges), adjList(_adjList) {
        d_0.resize(n);
        d_n.resize(n);
        algorithm(0, d_0); 
        algorithm(n - 1, d_n);
    }

    void algorithm(int source, vector<int>& d) {
        initialize_single_source(source, d);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
        Q.push({0, source});
        while (!Q.empty()) {
            int u = Q.top().second;
            int dist_u = Q.top().first;
            Q.pop();

            if (dist_u > d[u]) continue;

            for (const auto& edge : adjList[u]) {
                int v = edge.first;
                int w = edge.second;
                relax(u, v, w, Q, d);
            }
        }
    }

    void resolve() {
        int res = 0;
        int min_distance = d_0[n - 1];

        for (const Edge& edge : edges) {
            int v = edge.v;
            int u = edge.u;
            int w = edge.w;

            if (d_0[u] + w + d_n[v] == min_distance || d_0[v] + w + d_n[u] == min_distance) {
                res += (w * 2);
            }
        }

        cout << res << endl;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].v >> edges[i].u >> edges[i].w;
    }

    vector<vector<pair<int, int>>> adjList(n);
    for (const Edge& edge : edges) {
        int v = edge.v;
        int u = edge.u;
        int w = edge.w;
        adjList[v].push_back({u, w});
        adjList[u].push_back({v, w});
    }

    Dijkstra dijkstra(n, m, edges, adjList);
    dijkstra.resolve();

    return 0;
}