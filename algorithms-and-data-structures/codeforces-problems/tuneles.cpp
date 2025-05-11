/* 
/ 
/   En estos últimos años, Juli estuvo muy ocupado estudiando para exámenes y el concurso de Ay1.
/   Ahora que ya se recibió quiere relajarse y recorrer todo Exactas tranquilo.
/   Exactas consta de n aulas numeradas del 1 al n.
/   Juli comienza a caminar desde el aula número 1 (en particular, el aula 6 del Pabe II) y sigue alguna secuencia de aulas.
/   Caminar desde el espacio número i hasta otro espacio j requiere |i−j| unidades de energía.
/   La energía total gastada por Juli al visitar una secuencia de aulas e1=1,e2,…,ek es igual a ∑i=1k−1|ei−ei+1| unidades de energía.
/   Por supuesto, caminar sería aburrido si no hubiera atajos.
/   Un atajo es un túnel oculto que permite a Juli caminar de un aula a otra requiriendo solo 1 unidad de energía.
/   Hay exactamente n atajos en Exactas; el i-ésimo de ellos permite caminar desde el aula i hasta el aula ai (i≤ai≤ai+1) (pero no en la dirección opuesta),
/   por lo tanto, hay exactamente un atajo que comienza en cada aula.
/   Formalmente, si Juli elige una secuencia e1=1,e2,…,ek, entonces para cada 1≤i<k que satisface ei+1=aei y aei≠ei,
/   Juli gastará solo 1 unidad de energía en lugar de |ei−ei+1| al caminar desde el aula ei hasta el aula ei+1.
/   Por ejemplo, si Juli elige una secuencia e1=1,e2=ae1,e3=ae2,…,ek=aek−1, gastará exactamente k−1 unidades de energía total al recorrerlas.
/   Antes de emprender su aventura, Juli te pide que encuentres la cantidad mínima de energía requerida para alcanzar cada una de las intersecciones desde el aula inicial.
/   Formalmente, para cada 1≤i≤n, Juli está interesado en encontrar la mínima energía total posible de alguna secuencia e1=1,e2,…,ek=i.
/
/   Submission: https://codeforces.com/gym/551343/submission/293566149
/
*/
#include <iostream>
#include <vector>
#include <queue>

#define MAX 200002

using namespace std;

class Tuneles
{
private:
    int n;
    vector<int> tuneles;

    vector<int> setTuneles()
    {
        vector<int> tuneles(n);

        for (int i = 0; i < n; i++)
        {
            cin >> tuneles[i];
        }

        return tuneles;
    }

public:
    Tuneles(int n) : n(n)
    {
        resolve();
    }

    void updateValue(vector<int> &energias, queue<int> &queue, int newValue, int position)
    {
        if (position < 0 || position > n)
        {
            return;
        }

        if (energias[position] > newValue)
        {
            energias[position] = newValue;
            queue.push(position);
        }
    }

    void resolve()
    {
        vector<int> tuneles = setTuneles();
        vector<int> energia(n, MAX);

        energia[0] = 0;

        queue<int> cola;
        cola.push(0);

        while (!cola.empty())
        {
            int curr = cola.front();
            cola.pop();
            int newPossibleValue = energia[curr] + 1;

            for (int i = curr - 1; i <= curr + 1; i += 2)
            {
                updateValue(energia, cola, newPossibleValue, i);
            }

            int tunnelExit = tuneles[curr] - 1;

            updateValue(energia, cola, newPossibleValue, tunnelExit);
        }

        for (int i = 0; i < n; i++)
        {
            cout << energia[i] << " ";
        }

        cout << endl;

        return;
    }
};

int main()
{
    int n; // cantidad de aulas
    cin >> n;

    Tuneles tuneles(n);

    return 0;
}