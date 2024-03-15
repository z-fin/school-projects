package aed;

public class IPv4MergeSort extends MergeSort<IPv4Address> {

    @Override
    protected IPv4Address[] createArray(int length) {
        return new IPv4Address[length];
    }
    
}
