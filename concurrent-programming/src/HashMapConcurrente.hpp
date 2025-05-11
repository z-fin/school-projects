#ifndef HMC_HPP
#define HMC_HPP

#include <array>
#include <atomic>
#include <mutex>
#include <string>
#include <vector>

#include "ListaAtomica.hpp"
using namespace std;  // then you can use mutex directly

typedef std::pair<std::string, unsigned int> hashMapPair;

class HashMapConcurrente {
   public:
    static constexpr int cantLetras = 26;

    HashMapConcurrente();

    void incrementar(std::string clave);
    std::vector<std::string> claves();
    unsigned int valor(std::string clave);
    float promedio();
    float promedioParalelo(unsigned int cantThreads);

   private:
    ListaAtomica<hashMapPair>* tabla[HashMapConcurrente::cantLetras];

    vector<mutex> mutexHash{26};  // para c/ letra un mutex
    mutex mutexPromedio;
    static unsigned int hashIndex(std::string clave);
    void rutinaHiloPromedio(atomic<int>& primeraLetraDisponible,
                            atomic<int>& acumulador,
                            atomic<int>& cantidadDeClaves);
};

#endif /* HMC_HPP */
