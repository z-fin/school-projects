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
// Tests Ejercicio 1

LT_BEGIN_SUITE(TestsEjercicio1)
void set_up() {
    cout << "TEST EJERCICIO 1" << endl;
}

void tear_down() {
}

LT_END_SUITE(TestsEjercicio1)

void insertarNumerosMuchos(ListaAtomica<int> &l, unsigned int inicio, unsigned int fin) {
    for (unsigned int i = inicio; i < fin; ++i) {
        // Puedes insertar números duplicados, por ejemplo, inserta el número 'inicio'
        for (unsigned int j = 0; j < 3; ++j) {  // Cambia 3 por el número de repeticiones deseadas
            l.insertar(i);
        }
    }
}

void insertarNumeros(ListaAtomica<int> &l, int num) {
    l.insertar(num);
}


LT_BEGIN_TEST(TestsEjercicio1, ListaComienzaVacia)
    ListaAtomica<int> l;
    LT_CHECK_EQ(l.longitud(), 0);
LT_END_TEST(ListaComienzaVacia)


LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElemento)
    ListaAtomica<int> l;
    l.insertar(42);
    LT_CHECK_EQ(l.longitud(), 1);
LT_END_TEST(InsertarAgregaElemento)


LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElementoCorrecto)
    ListaAtomica<int> l;
    l.insertar(42);
    LT_CHECK_EQ(l[0], 42);
LT_END_TEST(InsertarAgregaElementoCorrecto)


LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaEnOrden)
    ListaAtomica<int> l;
    l.insertar(4);
    l.insertar(3);
    l.insertar(2);
    l.insertar(1);
    LT_CHECK_EQ(l.longitud(), 4);
    LT_CHECK_EQ(l[0], 1);
    LT_CHECK_EQ(l[1], 2);
    LT_CHECK_EQ(l[2], 3);
    LT_CHECK_EQ(l[3], 4);
LT_END_TEST(InsertarAgregaEnOrden)


LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaEnOrdenMuchos)
    ListaAtomica<int> l;
    for (int i = 0; i < 100; i++) {
        // cout << "Primera longitud: " << l.longitud() << endl;
        l.insertar(i);
        // cout << "Luego de insertar longitud: " << l.longitud() << endl;
    }

    for (int i = 0; i < 100; i++) {
        // Chequea que el valor en la posición i sea igual a 9 - i
        LT_CHECK_EQ(l[i], 99 - i);
    }
LT_END_TEST(InsertarAgregaEnOrdenMuchos)


LT_BEGIN_TEST(TestsEjercicio1, ConcurrentInsertar)
    ListaAtomica<int> l;
    vector<int> numerosEsperados;

    const int numHilos = 10;
    vector<thread> hilos;

    // creamos hilos
    for (int i = 0; i < numHilos; ++i) {
        hilos.emplace_back(insertarNumeros, ref(l), i);
        numerosEsperados.push_back(i);
    }

    // tesperamos que todos los hilos terminen
    for (auto &h : hilos) {
        h.join();
    }

    LT_CHECK_EQ(l.longitud(), numHilos);

    // chequeo si están todos los elementos (?)
    for (auto num : numerosEsperados) {
        bool encontrado = false;
        for (unsigned int i = 0; i < l.longitud(); i++) {
            if (l[i] == num) {
                encontrado = true;
                break;
            }
        }
        LT_CHECK(encontrado);  // Asegurarse de que cada número esté en la lista
    }
LT_END_TEST(ConcurrentInsertar)


LT_BEGIN_TEST(TestsEjercicio1, ConcurrentInsertarMuchos)
    ListaAtomica<int> l;

    const int numHilos = 10;
    const int elementosPorHilo = 10;

    vector<thread> hilos;
    vector<int> numeros;

    for (int i = 0; i < numHilos * elementosPorHilo; i++) {
        numeros.push_back(i);
    }

    // creamos hilos
    for (int i = 0; i < numHilos; ++i) {
        hilos.emplace_back(insertarNumerosMuchos, ref(l), i * elementosPorHilo, (i + 1) * elementosPorHilo);
    }

    // espero a que terminen
    for (auto &h : hilos) {
        h.join();
    }

    // chequeo si está bien la longitud
    LT_CHECK_EQ(l.longitud(), numHilos *elementosPorHilo * 3);

    // tipo diccionario
    unordered_map<int, int> verificador;

    for (long unsigned int i = 0; i < l.longitud(); i++) {
        int num = l[i];
        verificador[num]++;
    }

    // Comparar los contadores
    for (const auto &par : numeros) {
        LT_CHECK_EQ(verificador[par], 3);  // Debe ser 3 para cada número
    }
LT_END_TEST(ConcurrentInsertarMuchos)
// Ejecutar tests


LT_BEGIN_AUTO_TEST_ENV()
AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
