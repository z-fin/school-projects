#include <atomic>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <unordered_map>
#include <utility>  // Para pair
#include <vector>

#include "../src/CargarArchivos.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/ListaAtomica.hpp"
#include "lib/littletest.hpp"

using namespace std;

LT_BEGIN_SUITE(TestsEjercicio3)

void set_up() {
    cout << "TEST EJERCICIO 3" << endl;
}

void tear_down() {
}


LT_END_SUITE(TestsEjercicio3)
void insertarPalabras(HashMapConcurrente &hM, vector<string> &palabras, int cantDeVeces) {
    for (int i = 0; i < cantDeVeces; i++) {
        for (auto elem : palabras) {
            hM.incrementar(elem);
        }
    }
}

void promedioConcurrente(HashMapConcurrente &hM, promise<float> prom) {
    float res = hM.promedioParalelo(10);
    prom.set_value(res);
}

LT_BEGIN_TEST(TestsEjercicio3, PromedioEsCorrecto)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Microondas");

    float actual = hM.promedio();
    LT_CHECK_EQ(actual, 3);
LT_END_TEST(PromedioEsCorrecto)


LT_BEGIN_TEST(TestsEjercicio3, PromedioEsCorrectoMasClaves)
    HashMapConcurrente hM;
    vector<string> paraAgregar = {
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew"};

    for (long unsigned int i = 0; i < paraAgregar.size(); i++) {
        hM.incrementar(paraAgregar[i]);
    }

    float actual = hM.promedio();

    LT_CHECK_EQ(actual, 6);
LT_END_TEST(PromedioEsCorrectoMasClaves)


LT_BEGIN_TEST(TestsEjercicio3, PromedioConcurrenteEsCorrecto)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Microondas");

    float actual = hM.promedioParalelo(3);
    LT_CHECK_EQ(actual, 3);
LT_END_TEST(PromedioConcurrenteEsCorrecto)


LT_BEGIN_TEST(TestsEjercicio3, PromedioConcurrenteMasHilosQUeLetras)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Microondas");

    float actual = hM.promedioParalelo(30);
    LT_CHECK_EQ(actual, 3);
LT_END_TEST(PromedioConcurrenteMasHilosQUeLetras)


LT_BEGIN_TEST(TestsEjercicio3, PromedioConcurrenteDiezHilosPidiendoYAgregando)
    HashMapConcurrente hM;

    int numHilos = 10;
    vector<string> paraAgregar = {
        "arbol", "cielo", "mariposa", "computadora", "montania",
        "rio", "estrella", "oceano", "felicidad", "sabiduria",
        "musica", "luz", "sombra", "viaje", "sueño",
        "flor", "amor", "nube", "paz", "libertad",
        "sonrisa", "aventura", "lago", "paisaje", "recuerdo",
        "historia", "creatividad", "naturaleza", "tecnología", "arte",
        "desierto", "ciudad", "familia", "esperanza", "luna"};

    vector<thread> hilos;
    vector<future<float>> futuros;

    // Creación de hilos
    for (int i = 0; i < numHilos; i++) {
        promise<float> prom;
        futuros.emplace_back(prom.get_future());
        hilos.emplace_back(insertarPalabras, ref(hM), ref(paraAgregar), i + 1);
        hilos.emplace_back(promedioConcurrente, ref(hM), move(prom));
    }

    // Esperar y ver resultados
    for (auto &futuro : futuros) {
        float resultado = futuro.get();  // Bloquea hasta obtener el resultado
        bool res = resultado <= 55;
        LT_CHECK_EQ(res, true);
    }
    // espero a que terminen
    for (auto &h : hilos) {
        h.join();
    }

    // chequeo de que todo se agregó bien
    vector<string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), paraAgregar.size());
    for (string &palabra : paraAgregar) {

        LT_CHECK_EQ(hM.valor(palabra), 55);
    }

    LT_CHECK_EQ(hM.promedioParalelo(10), 55);
    LT_CHECK_EQ(hM.promedio(), 55);

LT_END_TEST(PromedioConcurrenteDiezHilosPidiendoYAgregando)

// Ejecutar tests
LT_BEGIN_AUTO_TEST_ENV()
AUTORUN_TESTS();
LT_END_AUTO_TEST_ENV()