package aed;

// elem1.compareTo(elem2) returns an integer; if it's greater than 0, then elem1 > elem2
public class BST<T extends Comparable<T>> implements Set<T> {
    private int size;
    private Node root;

    private class Node {

        public Node parent;
        public Node childLeft;
        public Node childRight;
        public T value;
       
        Node(T value) {
            this.value = value;
            this.parent = null;
            this.childLeft = null;
            this.childRight = null;
        }

        public boolean isLeftChild() {
            if (this.parent == null) {
                return false;
            }

            return this.parent.childLeft == this;
        }
    }

    public BST() {
        size = 0;
        root = null;
    }

    public int cardinal() {
        return size;
    }

    public T min(){
        if (root == null) {
            return null;
        }

        return minPriv(root).value;
    }

    private Node minPriv(Node currNode) {
        while (currNode.childLeft != null) {
            currNode = currNode.childLeft;
        }
        return currNode;        
    }
    
    public T max(){
        if (root == null) {
            return null;
        }

        return maxPriv(root).value;
    }

    private Node maxPriv(Node currNode) {
        while (currNode.childRight != null) {
            currNode = currNode.childRight;
        }
        return currNode;
    }

    public void add(T value){
        if (size == 0) {
            root = new Node(value);
            size = 1;
            
        } else {
            Node currNode = root;
            boolean added = false;

            while (!added) {
                if (currNode.value.compareTo(value) < 0) {  //if curNodo.value > value
                    if (currNode.childRight == null) {
                        currNode.childRight = new Node(value);
                        currNode.childRight.parent = currNode;
                        added = true;
                        size += 1;
                    } else {
                        currNode = currNode.childRight;
                    }

                } else if(currNode.value.compareTo(value) > 0) { //if curNodo.value > value
                    if (currNode.childLeft == null) {
                        currNode.childLeft = new Node(value);
                        currNode.childLeft.parent = currNode;
                        added = true;
                        size += 1;
                    } else {
                        currNode = currNode.childLeft;
                    }

                } else {
                    return;
                    
                }
            }
        }
    }

    public boolean belongs(T value){
        return (belongsPriv(value) != null);
    }

    private Node belongsPriv(T value) {
        Node currNode = root;

        while(currNode != null) {
            int compareResult = currNode.value.compareTo(value);

            if (compareResult == 0) { //if curNodo.value == value
                return currNode;
            }

            if (compareResult < 0) {  //if curNodo.value > value
                currNode = currNode.childRight;
            } else if(compareResult > 0) { //if curNodo.value > value
                currNode = currNode.childLeft;
            }
        }
        return null;
    }

    public void remove(T value){
        Node currNode = belongsPriv(value);
        
        if (currNode == null) {
            return;
        }

        if (currNode.childRight == null && currNode.childLeft == null) { // no children
            if (currNode == root) {
                root = null;
                size = 0;
                return;
            }

            if (currNode.isLeftChild()) {
                currNode.parent.childLeft = null;
                currNode.parent = null;
                size -= 1;
            } else {
                currNode.parent.childRight = null;
                currNode.parent = null;
                size -= 1;
            }
            return;
        }

        if (currNode.childRight != null && currNode.childLeft == null) { // 1 child (right)
            if (currNode == root) {
                root = currNode.childRight;
                currNode.childRight.parent = null;
                size -= 1;
                return;
            }

            if (currNode.isLeftChild()) {
                currNode.parent.childLeft = currNode.childRight;
                currNode.childRight.parent = currNode.parent;
                currNode.parent = null;
                size -= 1;
                return;
            } else {
                currNode.parent.childRight = currNode.childRight;
                currNode.childRight.parent = currNode.parent;
                currNode.parent = null;
                size -= 1;
                return;
            }
        }

        if (currNode.childLeft != null && currNode.childRight == null) { // 1 child (left)
            if (currNode == root) {
                root = currNode.childLeft;
                currNode.childLeft.parent = null;
                size -= 1;
                return;
            }

            if (currNode.isLeftChild()) {
                currNode.parent.childLeft = currNode.childLeft;
                currNode.childLeft.parent = currNode.parent;
                currNode.parent = null;
                size -= 1;
                return;
            } else {
                currNode.parent.childRight = currNode.childLeft;
                currNode.childLeft.parent = currNode.parent;
                currNode.parent = null;
                size -= 1;
                return;
            }
        }

        if (currNode.childRight != null && currNode.childLeft != null) { // 2 children
            Node inOrderPred = maxPriv(currNode.childLeft);
            remove(inOrderPred.value);
            currNode.value = inOrderPred.value;
            return; 
        }

    }

    public String toString(){
        String res = "{";
        Iterator<T> iterator = iterator();
        while (iterator.existsNext()){
            res += iterator.next() + ",";
        }
        res = res.substring(0,res.length() - 1);
        res += "}";
        return res;
    }

    private class ABB_Iterator implements Iterator<T> {
        private Node _current = minPriv(root);

        public boolean existsNext() {            
            return _current != null;
        }
    
        public T next() {
            if (_current == null) {
                return null;
            }

            T value = _current.value;
            if (_current.childRight != null) {
                _current = minPriv(_current.childRight);
            } else {
                if (_current.isLeftChild()) {
                    _current = _current.parent;
                } else {
                    while (_current != null && !_current.isLeftChild()) {
                        _current = _current.parent;
                    }
                    if (_current != null) {
                        _current = _current.parent;
                    }
                }
            }

            return value;
        }
    }

    public Iterator<T> iterator() {
        return new ABB_Iterator();
    }

}
