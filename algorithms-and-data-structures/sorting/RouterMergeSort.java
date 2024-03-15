package aed;

public class RouterMergeSort extends MergeSort<Router> {
    @Override
    protected Router[] createArray(int length) {
        return new Router[length];
    }
}
