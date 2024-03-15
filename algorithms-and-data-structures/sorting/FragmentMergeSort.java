package aed;

public class FragmentMergeSort extends MergeSort<Fragment> {

    @Override
    protected Fragment[] createArray(int length) {
        return new Fragment[length];
    }
    
}