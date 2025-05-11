package aed;

public class Heap {
    private Partido[] array;
    private int cardinalActual;

    Heap (int cardinalMaximo, Partido[] nuevoArray) {
        this.array = nuevoArray;
        this.cardinalActual = cardinalMaximo;

        int posicionActual = cardinalActual - 1;
        while (0 <= posicionActual) { // Ordenamos el arreglo con algoritmo de floyd => O(P)
            bajarEnElArbol(posicionActual);
            posicionActual--;
        }
    }

    public Partido[] getArray() {
        return array; // O(1)
    }

    public Partido maximo(){
        return array[0]; // O(1)
    }
    public int hijoDer(int indice) {
        return (indice * 2) + 2; // O(1)
    }
    public int hijoIzq(int indice) {
        return (indice * 2) + 1; // O(1)
    }
    public int padre(int indice) {
        return (indice / 2); // O(1)
    }

    public boolean esHoja(int indice) {
        return indice >= (cardinalActual / 2); // O(1)
    }

    public void intercambiarPosiciones(int adelante, int atras) {
        Partido valor = array[adelante]; // O(1)
        array[adelante] = array[atras]; // O(1)
        array[atras] = valor; // O(1)
    }

    public void actualizarMaximo(int elem) {
        array[0].cociente = elem; // O(1)
        array[0].divisiones += 1; // O(1)
        bajarEnElArbol(0); // O(P)
        // Complejidad final: O(P)
    }

    public boolean esMasChicoQueAlgunHijo(int indice){
        int hijoDer = hijoDer(indice); // O(1)
        int hijoIzq = hijoIzq(indice); // O(1)
        return (array[hijoDer] != null && array[indice].cociente < array[hijoDer].cociente) ||
                (array[hijoIzq] != null && array[indice].cociente < array[hijoIzq].cociente); // O(1)
    }

    public boolean bajarPorDerecha(int indice, int hijoDer, int hijoIzq){
        return  ((array[hijoDer] != null && array[hijoIzq] != null) &&
                (array[hijoIzq].cociente < array[hijoDer].cociente))
                || array[hijoIzq] == null && array[hijoDer].cociente > array[indice].cociente; // O(1)
    }

    public boolean bajarPorIzquierda(int indice, int hijoDer, int hijoIzq) {
        return  ((array[hijoIzq] != null && array[hijoDer] != null) &&
                (array[hijoDer].cociente < array[hijoIzq].cociente))
                || array[hijoDer] == null && array[hijoIzq].cociente > array[indice].cociente; // O(1)
    }
    public void bajarEnElArbol(int indice) {
        int hijoDer = hijoDer(indice); // O(1)
        int hijoIzq = hijoIzq(indice); // O(1)

        if (!esHoja(indice) && esMasChicoQueAlgunHijo(indice)) {  // O(1)

            if (bajarPorDerecha(indice, hijoDer, hijoIzq)) { // O(1)
                intercambiarPosiciones(indice, hijoDer); // O(1)
                bajarEnElArbol(hijoDer); // O(1)

            } else if (bajarPorIzquierda(indice, hijoDer, hijoIzq)) { // O(1)
                intercambiarPosiciones(indice, hijoIzq); // O(1)
                bajarEnElArbol(hijoIzq); // O(1)
            }
        }
        // Es recursivo, pero en el peor caso, bajo la altura del heap, O(log(n))
    }

}

