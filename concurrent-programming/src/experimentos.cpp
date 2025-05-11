#include <atomic>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "../src/CargarArchivos.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/ListaAtomica.hpp"

using namespace std;

int main() {
    vector<string> archivos;
    mutex mutexSalida;  // Mutex para proteger la escritura en el archivo de salida
    {
        ofstream outFile("tiempoCargar/todosLosTiempos.txt", ios::trunc);
        outFile.close();  // Al cerrarlo, el archivo queda limpio
    }

    // Generar los archivos 
    for (int i = 0; i < 100; i++) archivos.push_back("../data/corpus.txt");

    // Iterar sobre diferentes cantidades de hilos
    for (unsigned int threads = 1; threads <= 120; threads++) {
        HashMapConcurrente hashMap;

        // Medir el tiempo
        auto start = chrono::high_resolution_clock::now();
        cargarMultiplesArchivos(hashMap, threads, archivos);
        auto end = chrono::high_resolution_clock::now();

        // Calcular duración en milisegundos
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        // Proteger el acceso al archivo
        lock_guard<mutex> lock(mutexSalida);
        ofstream outFile("tiempoCargar/todosLosTiempos.txt", ios::app);  // Modo append
        outFile << "Cantidad de hilos: " << threads 
                << ", Tiempo: " << duration << " ms" << endl;
        outFile.close();
    }

    return 0;
}


// int main() {
//     HashMapConcurrente hashMap;

//     // Generar los archivos 
//     vector<string> archivos;
//     for (int i = 0; i < 20; i++) archivos.push_back("../data/corpus.txt");
//     cargarMultiplesArchivos(hashMap, 5, archivos);

//     return 0;
// }
