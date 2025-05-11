#ifndef CHM_CPP
#define CHM_CPP

#include "CargarArchivos.hpp"

#include <atomic>
#include <cctype>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

mutex mutexSalida;

int cargarArchivo(HashMapConcurrente &hashMap, string filePath) {
    fstream file;
    int cant = 0;
    string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo '" << filePath << "'" << endl;
        return -1;
    }
    while (file >> palabraActual) {
        // Completar (Ejercicio 4)
        if (!isdigit(palabraActual[0])) {
            hashMap.incrementar(palabraActual);
            cant++;
        }
    }
    // Cierro el archivo.
    if (!file.eof()) {
        cerr << "Error al leer el archivo" << endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

void cargarArchivoThread(HashMapConcurrente &hashMap,
                         vector<string> &filePaths,
                         atomic<unsigned int> &archivos_tomados) {

    unsigned int i = 0;

    while ((i = archivos_tomados.fetch_add(1)) < filePaths.size()) {
        cargarArchivo(hashMap, filePaths[i]);
    }
}

void cargarMultiplesArchivos(HashMapConcurrente &hashMap,
                             unsigned int cantThreads,
                             vector<string> filePaths) {
    atomic<unsigned int> archivosCargados{0};
    vector<thread> threads(cantThreads);

    // Crear y lanzar hilos
    for (unsigned int i = 0; i < cantThreads; i++) {
        threads[i] = thread(cargarArchivoThread, ref(hashMap), ref(filePaths), ref(archivosCargados));
    }

    // Esperar a que terminen los hilos
    for (unsigned int i = 0; i < cantThreads; i++) {
        threads[i].join();
    }
}

#endif
