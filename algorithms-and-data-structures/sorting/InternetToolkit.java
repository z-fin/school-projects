package aed;

public class InternetToolkit {
    public InternetToolkit() {
    }

    public Fragment[] tcpReorder(Fragment[] fragments) {
        Fragment[] output = copyArrayFragments(fragments);
        FragmentMergeSort sorter = new FragmentMergeSort();
        for (int i = 0; i < fragments.length - 1; i++) {
            Fragment currFragment = output[i];
            Fragment nextFragment = output[i + 1];
            if (currFragment.compareTo(nextFragment) > 0) {
                // Update the original array with the sorted subarray
                Fragment[] sortedSubArray = sorter.doSort(fragments);
                for (int j = 0; j < sortedSubArray.length; j++) {
                    output[j] = sortedSubArray[j];
                }
                break;
            }
        }
        return output;
    }
    
    private Fragment[] copyArrayFragments(Fragment[] src) {
        Fragment[] copy = new Fragment[src.length];
        System.arraycopy(src, 0, copy, 0, src.length);
        return copy;
    }
    
    public Router[] kTopRouters(Router[] routers, int k, int umbral) {
        RouterMergeSort sorter = new RouterMergeSort();
        Router[] sortedRouters = sorter.doSort(routers);
        int index = sortedRouters.length;

        for (int i = 0 ; (i < sortedRouters.length && i < k) ; i++) {
            index = sortedRouters.length - 1 - i;
            if (sortedRouters[index].getTraffic() < umbral) {
                index++;
                break;
            }
        }

        int outputLength = sortedRouters.length - index;
        Router[] output = new Router[outputLength];

        for (int j = 0 ; j < outputLength ; j++) {
            output[j] = sortedRouters[sortedRouters.length - 1 - j];
        }

        return output;
    }

    public IPv4Address[] sortIPv4(String[] ipv4) {
        IPv4MergeSort sorter = new IPv4MergeSort();
        IPv4Address[] ipv4Array = new IPv4Address[ipv4.length];

        for (int i = 0 ; i < ipv4Array.length ; i++) {
            ipv4Array[i] = new IPv4Address(ipv4[i]);
        }
        return sorter.doSort(ipv4Array);
    }

}