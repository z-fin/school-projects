package aed;

interface Iterator<T> {
    /**
     * Devuelve true si hay un elemento siguiente en la colección.
     * 
     */
    public boolean isNextNode();
    
    /**
     * Devuelve true si hay un elemento anterior en la colección.
     * 
     */
    public boolean isPrevNode();

    /**
     * Devuelve el elemento siguiente en la colección y avanza el iterador.
     * 
     */
    public T nextNode();

    /**
     * Devuelve el elemento anterior en la colección y retrocede el iterador.
     * 
     */
    public T previousNode();

}
