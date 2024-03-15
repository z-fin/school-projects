package aed;

interface Set<T> {

    public int cardinal();

    public T min();


    public T max();


    public void add(T elem);


    public boolean belongs(T elem);


    public void remove(T elem);

    public String toString();

}
