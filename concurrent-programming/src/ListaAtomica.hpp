#ifndef LISTA_ATOMICA_HPP
#define LISTA_ATOMICA_HPP

#include <atomic>
#include <cstddef>

template <typename T>
class ListaAtomica {
   private:
    struct Nodo {
        Nodo(const T &val) : _valor(val), _siguiente(nullptr) {}

        T _valor;
        Nodo *_siguiente;
    };

    std::atomic<Nodo *> _cabeza;

   public:
    ListaAtomica() : _cabeza(nullptr) {}

    ~ListaAtomica() {
        Nodo *n, *t;
        n = _cabeza.load();
        while (n) {
            t = n;
            n = n->_siguiente;
            delete t;
        }
    }

    void insertar(const T &valor) {
        Nodo *nuevoNodo = new Nodo(valor);
        nuevoNodo->_siguiente = _cabeza.load();
        while (!_cabeza.compare_exchange_weak(nuevoNodo->_siguiente, nuevoNodo));

        /**
         * busy waiting
         * compare_exchange_weak realiza lo siguiente:
            1. Compara el valor actual de _cabeza con nuevoNodo->_siguiente.
            2. Si son iguales:
                - Actualiza _cabeza para que apunte a nuevoNodo, convirtiéndolo en la nueva cabeza de la lista.
                _cabeza = nuevoNodo
            3. Si no son iguales (es decir, otro hilo cambió _cabeza):
                - Asigna el valor actual de _cabeza a nuevoNodo->_siguiente, asegurando que el nuevo nodo apunte 
                a la cabeza actual.
                nuevoNodo->_siguiente = _cabeza
            4. Retorna true si la operación tuvo éxito, o false si falló.
         */
    }

    T &operator[](size_t i) const {
        Nodo *n = _cabeza.load();
        for (size_t j = 0; j < i; j++) {
            n = n->_siguiente;
        }
        return n->_valor;
    }

    unsigned int longitud() const {
        Nodo *n = _cabeza.load();
        unsigned int cant = 0;
        while (n != nullptr) {
            cant++;
            n = n->_siguiente;
        }
        return cant;
    }

    struct iterator {
       private:
        ListaAtomica *_lista;

        typename ListaAtomica::Nodo *_nodo_sig;

        iterator(ListaAtomica<T> *lista, typename ListaAtomica<T>::Nodo *sig)
            : _lista(lista), _nodo_sig(sig) {}

       public:
        iterator &operator=(const typename ListaAtomica::iterator &otro) {
            _lista = otro._lista;
            _nodo_sig = otro._nodo_sig;
            return *this;
        }

        T &operator*() {
            return _nodo_sig->_valor;
        }

        iterator &operator++() {
            _nodo_sig = _nodo_sig->_siguiente;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        T *operator->() {
            return &_nodo_sig->_valor;  // Acceso al valor
        }

        bool operator==(const typename ListaAtomica::iterator &otro) const {
            return _lista->_cabeza.load() == otro._lista->_cabeza.load() && _nodo_sig == otro._nodo_sig;
        }

        bool operator!=(const typename ListaAtomica::iterator &otro) const {
            return !(*this == otro);
        }

        friend iterator ListaAtomica<T>::begin();
        friend iterator ListaAtomica<T>::end();
    };

    iterator begin() {
        return iterator(this, _cabeza);
    }

    iterator end() {
        return iterator(this, nullptr);
    }
};

#endif /* LISTA_ATOMICA_HPP */