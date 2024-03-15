package aed;

public abstract class MergeSort <T extends Comparable<T>> {
    public T[] doSort(T[] array) {
        int length = array.length;
        // base case
        if (length <= 1) {
            return array;
        }
        int midPoint = length / 2;
        T[] leftHalf = doSort(subArray(array, 0, midPoint - 1));
        T[] rightHalf = doSort(subArray(array, midPoint, array.length - 1));
        return merge(leftHalf, rightHalf);
    }

    protected abstract T[] createArray(int length);
    
    private T[] merge(T[] left, T[] right) {
        T[] output = createArray(left.length + right.length);
         
        int indexLeft = 0;
        int indexRight = 0;
        int indexOutput = 0;
    
        while (indexLeft < left.length && indexRight < right.length) {
            if (left[indexLeft].compareTo(right[indexRight]) < 0) {
                output[indexOutput] = left[indexLeft];
                indexLeft++;
            } else {
                output[indexOutput] = right[indexRight];
                indexRight++;
            }
            indexOutput++;
        }
    
        // Copy remaining elements from both arrays
        System.arraycopy(left, indexLeft, output, indexOutput, left.length - indexLeft);
        System.arraycopy(right, indexRight, output, indexOutput, right.length - indexRight);
    
        return output;
    }
    
    private T[] subArray(T[] src, int start, int end) {
        T[] output = createArray(end - start + 1);
        int indexSrc = start;
    
        for (int i = 0; i < output.length; i++) {
            output[i] = src[indexSrc];
            indexSrc++;
        }
    
        return output;
    }
}
