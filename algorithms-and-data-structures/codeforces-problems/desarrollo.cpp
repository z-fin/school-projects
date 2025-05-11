/*
/   Tomás quiere estudiar cómo fue cambiando el nivel de conectividad de la Ciudad de Buenos Aires a lo largo del tiempo.
/   Para ello, armó un digrafo G donde los n nodos denotan las esquinas de la ciudad, y donde los ejes se corresponden con calles dirigidas que unen estas esquinas.
/   Para estas esquinas conoce el orden cronológico en el que fueron agregadas al trazado de la ciudad.
/   Para estudiar la conectividad de la ciudad se propone la siguiente tarea:
/   va a ir eliminando las esquinas del grafo en el orden opuesto al que fueron creadas, y cada vez que borra una va a calcular la suma de todas las distancias entre todos los pares de esquinas que todavía están en el mapa.
/   De esta forma, estudiando como aumenta este valor cree poder entender cuáles esquinas fueron las más importantes para aumentar la conectividad de la ciudad entera.
/   Debemos ayudar a Tomás a completar la tarea que propuso.
/
/   Submission: https://codeforces.com/gym/562517/submission/293425084
/
*/
#include <iostream>
#include <vector>

using namespace std;

class Floyd_Warshall {
private:
    int n;
    vector<vector<int>> D;
    vector<long long> order;

public:
    Floyd_Warshall(int _n, vector<vector<int>>& _D, vector<long long> _order) : n(_n), D(_D), order(_order) {
    }

    vector<long long> algorithm(int n) {
        vector<long long> res;
        for (int k_in = 0; k_in < n; k_in++) {
            long long sum = 0;
            int k = order[k_in] - 1;
            for (int i= 0; i <n; i++) {
                for (int j= 0; j<n; j++) {
                    D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
                }
            }

            for (int i_in = 0; i_in <= k_in; i_in++) {
                int i = order[i_in] - 1;
                for (int j_in = 0; j_in <= k_in; j_in++) {
                    int j = order[j_in] - 1;
                    if (D[i][j] == -1) continue;
                        sum += D[i][j];
                }
            }
            res.push_back(sum);
        }
        return res;
    }
}; 

void reverse(vector<long long>& order) {
    vector<long long> reversed;

    for (int i = order.size() - 1 ; i >= 0; i--) {
        reversed.push_back(order[i]);
    }

    order = reversed;
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    vector<long long> orden(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> adjMatrix[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        cin >> orden[i];
    }

    reverse(orden);
    vector<vector<int>> currD = adjMatrix;
    Floyd_Warshall fw(n, currD, orden);
    vector<long long> res = fw.algorithm(n);
    reverse(res);

    for (int i = 0; i < n; i++) {
        cout << res[i] << " ";
    }
}