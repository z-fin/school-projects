package aed;

import java.util.*;

public class LinkedList<T> implements Sequence<T> {
    // Completar atributos privados
    private int length;
    private Node head; // Primer nodo
    private Node tail; // Segundo nodo


    private class Node {
        // Completar
        public T value;
        public Node nextNode;
        public Node previousNode;

        Node(T value) { //constructor
            this.value = value;
            this.nextNode = null;
            this.previousNode = null;
        }
    }

    public LinkedList() {
        head = null;
        tail = null;
        length = 0;
    }

    public int length() {
        return length;
    }

    public void addInFront(T value) {
       Node newNode = new Node(value); //Nodo a agregar

        if (head == null) {
            head = newNode;
            tail = head;
        } else {
            newNode.nextNode = head;
            head.previousNode = newNode;
            head = newNode;
        }

        length++;
    }

    public void addBehind(T value) {
        Node newNode = new Node(value);

        if (head == null) {
            head = newNode;
            tail = head;
        } else {
            newNode.previousNode = tail;
            tail.nextNode = newNode;
            tail = newNode;
        }

        length++;
    }

    public T get(int index) {
        if (index == 0) { // el head estara en i = 0
            return head.value;
        } else if (index == length - 1) { //El tail estara en i = longitud - 1 porque el indice empieza en 0
            return tail.value;
        } else {
            Node currNode = head.nextNode;

            int j = 1;
            while (j < index) {
                currNode = currNode.nextNode;
                j++;
            }
            
            return currNode.value;
        }
    }

    public void delete(int index) {
        if (index == 0) {
            head.nextNode.previousNode = head.nextNode;
            head = head.nextNode;
        } else if (index == length - 1) {
            tail.previousNode.nextNode = tail.previousNode;
            tail = tail.previousNode;
        } else {
            Node nodoActual = head.nextNode;

            for(int j = 1; j < index; j++) {
                nodoActual = nodoActual.nextNode;
            }

            nodoActual.previousNode.nextNode = nodoActual.nextNode;
            nodoActual.nextNode.previousNode = nodoActual.previousNode;
            nodoActual = nodoActual.previousNode;
        }
        length--;
    }

    public void setNewValue(int index, T newValue) {
        if (index == 0) {
            head.value = newValue;
        } else if (index == length - 1) {
            tail.value = newValue;
        } else {
            Node nodoActual = head.nextNode;

            int i = 1; // chequeo a partir del segundo elemento de la secuencia
            while (i < index) {
                nodoActual = nodoActual.nextNode;
                i++;
            }
            nodoActual.value = newValue;
        }
    }

    public LinkedList<T> copy() {
        LinkedList<T> copy = new LinkedList<T>();

        Node currNode = head;

        for (int i = 0; i < length; i++) {
            copy.addBehind(currNode.value);
            currNode = currNode.nextNode;
        }
        
        return copy;
    }

    public LinkedList(LinkedList<T> lista) {
        LinkedList<T> copyList = lista.copy();
        head = copyList.head;
        tail = copyList.tail;
        length = copyList.length();
    }
    
    @Override
    public String toString() {
        String res = "[" + head.value + ',';
        Node nodoActual = head.nextNode;

        int i = 1;
        while (i < length - 1) {
            res = res + " " + nodoActual.value + ",";
            nodoActual = nodoActual.nextNode;
            i ++;
        }

        res = res + " " + nodoActual.value + "]";
        return res;

    }

    private class ListIterator implements Iterator<T>{
    	// Completar atributos privados
        int pointer = 0;

        public boolean isNextNode() {
            return (pointer != length);
        }
        
        public boolean isPrevNode() {
            return (pointer != 0);
        }

        public T nextNode() {
            T currNode = get(pointer);
            if (isNextNode()) {
                pointer++;
            }
            return currNode;
        }
        

        public T previousNode() {
            T currNode = get(pointer - 1);
	        if (isPrevNode()) {
                pointer--;
            }
            return currNode;
        }
    }


    public Iterator<T> iterator() {
        return new ListIterator();
    }

}
