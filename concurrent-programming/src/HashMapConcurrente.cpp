#ifndef CHM_CPP
#define CHM_CPP

#include "HashMapConcurrente.hpp"

#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>  // Para ref
#include <iostream>
#include <thread>

using namespace std;

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(string clave) {
    return (unsigned int)(tolower(clave[0]) - 'a');
}

void HashMapConcurrente::incrementar(string clave) {
    unsigned int idx = hashIndex(clave);

    ListaAtomica<hashMapPair>* tablaActual = tabla[idx];

    // chequeo si falla
    if (tablaActual == nullptr) return;

    mutexHash[idx].lock();

    ListaAtomica<hashMapPair>::iterator it = tabla[idx]->begin();

    // ultimo elem de esa lista
    ListaAtomica<hashMapPair>::iterator ultimo = tabla[idx]->end();

    // avanzamos iterador
    while (it != ultimo && it->first != clave) {
        it++;
    }

    if (it != ultimo) {
        (it->second)++;  // Incrementa el valor si ya estaba
    } else {
        // Si no existe, creamos nueva
        hashMapPair nuevoPar(clave, 1);
        tabla[idx]->insertar(nuevoPar);
    }

    mutexHash[idx].unlock();
    return;
}

vector<string> HashMapConcurrente::claves() {
    vector<string> claves;

    for (unsigned int idx = 0; idx < cantLetras; idx++) mutexHash[idx].lock();

    // tenemos que recorrer c/letra todas las claves
    for (unsigned int idx = 0; idx < cantLetras; idx++) {
        for (auto& p : *tabla[idx]) {
            claves.push_back(p.first);
        }
        mutexHash[idx].unlock();
    }
    return claves;
}

unsigned int HashMapConcurrente::valor(string clave) {
    unsigned int idx = hashIndex(clave);

    ListaAtomica<hashMapPair>* tablaActual = tabla[idx];

    if (tabla[idx] == nullptr) return 0;

    // si no hay elementos en ese bucket entonces es cero, no recorremos nada
    if (tablaActual->longitud() == 0) {
        return 0;
    }

    // iterador comienza con el primero de la lista de ese bucket
    ListaAtomica<hashMapPair>::iterator it = tabla[idx]->begin();
    // ultimo elem de esa lista
    ListaAtomica<hashMapPair>::iterator ultimo = tabla[idx]->end();

    while (it != ultimo) {
        if (it->first == clave) {
            return it->second;
        }
        it++;
    }

    // si recorrimos todo y no estaba es cero
    return 0;
}

float HashMapConcurrente::promedio() {
    float sum = 0.0;
    unsigned int count = 0;

    for (unsigned int idx = 0; idx < cantLetras; idx++) mutexHash[idx].lock();

    for (unsigned int idx = 0; idx < cantLetras; idx++) {
        for (const auto& p : *tabla[idx]) {
            sum += p.second;
            count++;
        }
        mutexHash[idx].unlock();
    }
    if (count > 0) {
        return sum / count;
    }
    return 0;
}

void HashMapConcurrente::rutinaHiloPromedio(atomic<int>& primeraLetraDisponible, atomic<int>& acumulador, atomic<int>& cantidadDeClaves) {
    while (true) {
        int miLetra = primeraLetraDisponible.fetch_add(1);

        if (miLetra >= cantLetras) break;  // No hay más letras, termina el thread

        int localSum = 0;    // Suma local
        int localCount = 0;  // Conteo local

        // Recorro las claves de la fila miLetra
        for (const auto& p : *tabla[miLetra]) {
            localSum += p.second;
            localCount++;
        }
        mutexHash[miLetra].unlock();
        // Actualizo los acumuladores globales de forma atómica
        acumulador.fetch_add(localSum);
        cantidadDeClaves.fetch_add(localCount);
    }
}

float HashMapConcurrente::promedioParalelo(unsigned int cantThreads) {
    atomic<int> primeraLetraDisponible(0);
    atomic<int> acumulador(0);
    atomic<int> cantidadDeClaves(0);

    // Creamos threads
    vector<thread> threads;
    for (unsigned int idx = 0; idx < cantLetras; idx++) mutexHash[idx].lock();

    for (unsigned int i = 0; i < cantThreads; ++i) {
        threads.emplace_back(
            &HashMapConcurrente::rutinaHiloPromedio,
            this,
            ref(primeraLetraDisponible),
            ref(acumulador),
            ref(cantidadDeClaves));
    }

    // Esperamos que terminen los threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Calculamos el promedio final de forma segura
    int totalClaves = cantidadDeClaves.load();
    int totalSum = acumulador.load();

    return (totalClaves != 0) ? static_cast<float>(totalSum) / totalClaves : 0.0f;
}

#endif
