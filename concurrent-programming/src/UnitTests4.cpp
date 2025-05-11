#include <atomic>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <unordered_map>
#include <utility>  // Para std::pair
#include <vector>

#include "../src/CargarArchivos.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/ListaAtomica.hpp"
#include "lib/littletest.hpp"

using namespace std;

LT_BEGIN_SUITE(TestsEjercicio4)

void set_up() {
    cout << "TEST EJERCICIO 4" << endl;
}

void tear_down() {
}

LT_END_SUITE(TestsEjercicio4)

int contarPalabra(const std::vector<std::string>& vec, const std::string& palabra) {
    int contador = 0;
    for (const auto& str : vec) {
        if (str == palabra) {
            contador++;
        }
    }
    return contador;
}

tuple<vector<string>, unsigned int> vectorPalabras(string path) {
    ifstream file(path);
    vector<string> palabras;
    set<string> elementosUnicos;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string palabra;

        // Dividimos la línea en palabras
        while (ss >> palabra) {
            if (!isdigit(palabra[0])) {
                palabras.push_back(palabra);
                elementosUnicos.insert(palabra);
            }
        }
    }
    unsigned int contadorUnico = elementosUnicos.size();

    return make_tuple(palabras, contadorUnico);
}

void insertarPalabras(HashMapConcurrente& hM, string path) {
    cargarMultiplesArchivos(hM, 5, {path, path, path, path});
}

LT_BEGIN_TEST(TestsEjercicio4, CargarArchivoFunciona)
    HashMapConcurrente hM;
    cargarArchivo(hM, "data/test-1");
    LT_CHECK_EQ(hM.valor("Heladera"), 1);
    LT_CHECK_EQ(hM.claves().size(), 5);
LT_END_TEST(CargarArchivoFunciona)

LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaUnThread)
    HashMapConcurrente hM;
    cargarMultiplesArchivos(hM, 5, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("NotebookOMEN"), 3);
    LT_CHECK_EQ(hM.valor("Microondas"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaUnThread)


LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaDosThreads)
    HashMapConcurrente hM;
    cargarMultiplesArchivos(hM, 2, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("NotebookOMEN"), 3);
    LT_CHECK_EQ(hM.valor("Microondas"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaDosThreads)


LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaCincoThreads)
    HashMapConcurrente hM;
    cargarMultiplesArchivos(hM, 5, {"data/test-4.txt", "data/test-4.txt", "data/test-4.txt", 
                                    "data/test-4.txt", "data/test-4.txt", "data/test-4.txt", 
                                    "data/test-4.txt", "data/test-4.txt", "data/test-4.txt"});

    tuple<vector<string>, unsigned int> dato = vectorPalabras("data/test-4.txt");

    vector<string> palabras = get<0>(dato);
    unsigned int contadorUnicos = get<1>(dato);

    for (auto palabra : palabras) {
        int contador = contarPalabra(palabras, palabra) * 9;
        unsigned int valor = hM.valor(palabra);
        LT_CHECK_EQ(valor, contador);
    }

    LT_CHECK_EQ(hM.claves().size(), contadorUnicos);
LT_END_TEST(CargarMultiplesArchivosFuncionaCincoThreads)


LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaCorpus)
    HashMapConcurrente hM;
    cargarMultiplesArchivos(hM, 10, {"data/corpus.txt", "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", 
                                    "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", 
                                    "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", 
                                    "data/corpus.txt", "data/corpus.txt", "data/corpus.txt", 
                                    "data/corpus.txt", "data/corpus.txt", "data/corpus.txt"});

    tuple<vector<string>, unsigned int> dato = vectorPalabras("data/corpus.txt");

    vector<string> lineas = get<0>(dato);
    unsigned int contadorUnicos = get<1>(dato);

    for (auto linea : lineas) {
        int contador = contarPalabra(lineas, linea) * 18;
        unsigned int valor = hM.valor(linea);
        LT_CHECK_EQ(valor, contador);
    }

    LT_CHECK_EQ(hM.claves().size(), contadorUnicos);
LT_END_TEST(CargarMultiplesArchivosFuncionaCorpus)


LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaDosHilos)
    HashMapConcurrente hM;

    int numHilos = 2;

    vector<thread> hilos;
    vector<int> numeros;

    // creamos hilos
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back(insertarPalabras, ref(hM), "data/corpus.txt");
    }

    // espero a que terminen
    for (auto& h : hilos) {
        h.join();
    }

    tuple<vector<string>, unsigned int> dato = vectorPalabras("data/corpus.txt");

    vector<string> lineas = get<0>(dato);
    unsigned int contadorUnicos = get<1>(dato);

    vector<string> todasLasClaves = hM.claves();
    LT_CHECK_EQ(contadorUnicos, todasLasClaves.size());


    for (auto linea : lineas) {
        int contador = contarPalabra(lineas, linea) * 2 * 4;  // tenemos 2 hilos y c/ lo agrega 4 veces
        unsigned int valor = hM.valor(linea);
        LT_CHECK_EQ(valor, contador);
    }

LT_END_TEST(CargarMultiplesArchivosFuncionaDosHilos)


LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaDiezHilos)
    HashMapConcurrente hM;

    int numHilos = 10;

    vector<thread> hilos;
    vector<int> numeros;

    // creamos hilos
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back(insertarPalabras, ref(hM), "data/corpus.txt");
    }

    // espero a que terminen
    for (auto& h : hilos) {
        h.join();
    }

    tuple<vector<string>, unsigned int> dato = vectorPalabras("data/corpus.txt");

    vector<string> lineas = get<0>(dato);
    unsigned int contadorUnicos = get<1>(dato);

    vector<string> todasLasClaves = hM.claves();
    LT_CHECK_EQ(contadorUnicos, todasLasClaves.size());

    for (auto linea : lineas) {
        int contador = contarPalabra(lineas, linea) * 10 * 4;  // tenemos 10 hilos y c/ lo agrega 4 veces
        unsigned int valor = hM.valor(linea);
        LT_CHECK_EQ(valor, contador);
    }

LT_END_TEST(CargarMultiplesArchivosFuncionaDiezHilos)

// Ejecutar tests
LT_BEGIN_AUTO_TEST_ENV()
AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
