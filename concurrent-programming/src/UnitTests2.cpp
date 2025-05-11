#include <atomic>
#include <cstddef>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../src/CargarArchivos.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/ListaAtomica.hpp"
#include "lib/littletest.hpp"

using namespace std;

// Tests Ejercicio 2

LT_BEGIN_SUITE(TestsEjercicio2)

void set_up() {

    cout << "TEST EJERCICIO 2" << endl;
    // Initialize your HashMap or other necessary setup
}

void tear_down() {
    // Cleanup if necessary
}

LT_END_SUITE(TestsEjercicio2)


int contarPalabra(const std::vector<std::string>& vec, const std::string& palabra) {
    int contador = 0;
    for (const auto& str : vec) {
        if (str == palabra) {
            contador++;
        }
    }
    return contador;
}

void insertarPalabras(HashMapConcurrente &hM, vector<string> palabras) {
    for (auto elem : palabras) {
        hM.incrementar(elem);
    }
}

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoEnHashMapVacio)
    HashMapConcurrente hM;
    LT_CHECK_EQ(hM.valor("heladera"), 0);
LT_END_TEST(ValorEsCorrectoEnHashMapVacio)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoEnHashMapVacio)
    HashMapConcurrente hM;
    vector<string> actual = hM.claves();
    vector<string> expected = {};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoEnHashMapVacio)


LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasUnaInsercion)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 1);
LT_END_TEST(ValorEsCorrectoTrasUnaInsercion)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasUnaInsercion)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    vector<string> actual = hM.claves();
    vector<string> expected = {"Heladera"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasUnaInsercion)


LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasDosInsercionesMismaPalabra)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
LT_END_TEST(ValorEsCorrectoTrasDosInsercionesMismaPalabra)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasDosInsercionesMismaPalabra)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    vector<string> actual = hM.claves();
    vector<string> expected = {"Heladera"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasDosInsercionesMismaPalabra)


LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesMismoBucket)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Lavarropa");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("Lavarropa"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesMismoBucket)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesMismoBucket)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Lavarropa");
    vector<string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(find(actual.begin(), actual.end(), "Heladera") != actual.end());
    LT_CHECK(find(actual.begin(), actual.end(), "Lavarropa") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesMismoBucket)


LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesDistintoBucket)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("Microondas"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesDistintoBucket)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)
    HashMapConcurrente hM;
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Heladera");
    vector<string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(find(actual.begin(), actual.end(), "Heladera") != actual.end());
    LT_CHECK(find(actual.begin(), actual.end(), "Microondas") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoIntensivo)
    HashMapConcurrente hM;
    vector<string> paraAgregar = {
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "fig", "nectarine", "mango", "papaya", "quince",
        "raspberry", "strawberry", "tangerine", "ugli", "vanilla",
        "watermelon", "xigua", "yam", "blueberry", "cantaloupe",
        "dragonfruit", "elderberry", "jackfruit", "lime", "melon",
        "persimmon", "plum", "pomegranate", "coconut", "currant",
        "pumpkin", "radish", "spinach", "apricot", "grapefruit"};

    for (long unsigned int i = 0; i < paraAgregar.size(); i++) {
        hM.incrementar(paraAgregar[i]);
    }
    for (string &palabra : paraAgregar) {
        while (hM.valor(palabra) < 10) {
            hM.incrementar(palabra);
        }
    }
    vector<string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), paraAgregar.size());
    for (string &palabra : paraAgregar) {
        LT_CHECK_EQ(hM.valor(palabra), 10);
    }
    LT_END_TEST(ClavesEsCorrectoIntensivo)

    LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoIntensivoMayusculas)
    HashMapConcurrente hM;
    vector<string> paraAgregar = {
        "AppLe", "zuCcHini", "banana", "kiwi", "cHerry",
        "leMon", "dAte", "gRape", "orangE", "honEydew",
        "Fig", "nectArine", "mAngo", "papAya", "qUince",
        "rAspBerry", "straWberry", "tAngerIne", "ugLi", "vaNilLa",
        "waTermeLOn", "xigUA", "yam", "blUebErry", "canTaloUpe",
        "draGOnfRuit", "elderberry", "jackfruit", "lime", "melon",
        "PersimMon", "pLum", "pomegranate", "coconut", "currant",
        "PuMpkin", "raDISh", "SPInach", "AprIcot", "graPefrUit"};

    for (long unsigned int i = 0; i < paraAgregar.size(); i++) {
        hM.incrementar(paraAgregar[i]);
    }
    for (string &palabra : paraAgregar) {
        while (hM.valor(palabra) < 10) {
            hM.incrementar(palabra);
        }
    }
    vector<string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), paraAgregar.size());
    for (string &palabra : paraAgregar) {
        LT_CHECK_EQ(hM.valor(palabra), 10);
    }
LT_END_TEST(ClavesEsCorrectoIntensivoMayusculas)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoHilos)
    HashMapConcurrente hM;
    vector<string> paraAgregar = {"apple", "zucchini", "banana", "kiwi"};

    const int numHilos = 10;

    vector<thread> hilos;
    vector<int> numeros;

    // creamos hilos
    for (int i = 0; i < numHilos; ++i) {
        hilos.emplace_back(insertarPalabras, ref(hM), paraAgregar);
    }

    // espero a que terminen
    for (auto &h : hilos) {
        h.join();
    }

    vector<string> claves = hM.claves();

    LT_CHECK_EQ(claves.size(), 4);

    for (auto clave : claves) {
        LT_CHECK_EQ(hM.valor(clave), 10);
    }

LT_END_TEST(ClavesEsCorrectoHilos)


LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoHilosIntensivo)
    HashMapConcurrente hM;
    vector<string> paraAgregar = {
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "fig", "nectarine", "mango", "papaya", "quince",
        "raspberry", "strawberry", "tangerine", "ugli", "vanilla",
        "watermelon", "xigua", "yam", "blueberry", "cantaloupe",
        "dragonfruit", "elderberry", "jackfruit", "lime", "melon",
        "persimmon", "plum", "pomegranate", "coconut", "currant",
        "pumpkin", "radish", "spinach", "apricot", "grapefruit"
        };

    const int numHilos = 10;

    vector<thread> hilos;
    vector<int> numeros;

    // creamos hilos
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back(insertarPalabras, ref(hM), paraAgregar);
    }

    // espero a que terminen
    for (auto &h : hilos) {
        h.join();
    }

    vector<string> claves = hM.claves();

    LT_CHECK_EQ(claves.size(), paraAgregar.size());

    for (auto clave : claves) {
        int contador = contarPalabra(claves, clave); 
        LT_CHECK_EQ(hM.valor(clave), contador * 10);
    }

LT_END_TEST(ClavesEsCorrectoHilosIntensivo)


LT_BEGIN_TEST(TestsEjercicio2, EsCorrectoHilosIntensivoPartido)
    HashMapConcurrente hM;
    vector<string> paraAgregar1 = {
        "apple", "zucchini", "banana", "kiwi", "cherry",
        "lemon", "date", "grape", "orange", "honeydew",
        "fig", "nectarine", "mango", "papaya", "quince",
        "raspberry", "strawberry", "tangerine", "ugli", "vanilla",
        "watermelon", "xigua", "yam", "blueberry"};

    vector<string> paraAgregar2 = {
        "cantaloupe", "dragonfruit", "elderberry", "jackfruit", "lime", "melon",
        "persimmon", "plum", "pomegranate", "coconut", "currant",
        "pumpkin", "radish", "spinach", "apricot", "grapefruit"};

    int numHilos = 10;

    vector<thread> hilos;
    vector<int> numeros;

    // creamos hilos
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back(insertarPalabras, ref(hM), paraAgregar1);
    }
    for (int i = 0; i < numHilos; i++) {
        hilos.emplace_back(insertarPalabras, ref(hM), paraAgregar2);
    }

    // espero a que terminen
    for (auto &h : hilos) {
        h.join();
    }

    vector<string> claves = hM.claves();

    LT_CHECK_EQ(claves.size(), paraAgregar1.size() + paraAgregar2.size());

    for (auto clave : claves) {
        LT_CHECK_EQ(hM.valor(clave), 10);
    }

LT_END_TEST(EsCorrectoHilosIntensivoPartido)

// Ejecutar tests
LT_BEGIN_AUTO_TEST_ENV()
AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()