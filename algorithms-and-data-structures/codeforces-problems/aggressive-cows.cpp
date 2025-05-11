/*
/
/   Farmer John has built a new long barn, with N (2 <= N <= 100,000) stalls.
/   The stalls are located along a straight line at positions x1 ... xN (0 <= xi <= 1,000,000,000).
/   His C (2 <= C <= N) cows don't like this barn layout and become aggressive towards each other once put into a stall.
/   To prevent the cows from hurting each other, FJ wants to assign the cows to the stalls, such that the minimum distance between any two of them is as large as possible.
/   What is the largest minimum distance?
/
/   Accepted submission: https://www.spoj.com/TDA242C1/files/src/33466351/
/
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Barn
{
private:
    int n;
    int c;
    vector<int> stalls;

public:
    Barn(int n, int c) : n(n), c(c)
    {
        stalls.resize(n);
    };

    void createValidStalls()
    {
        for (int i = 0; i < n; i++)
        {
            cin >> stalls[i];
        }
    }

        bool isValidDistance(int distance)
    {
        int cowsLeft = c - 1;
        int posOfLastPlacedCow = stalls[0];

        for (int i = 1; i < n; i++)
        {
            if (stalls[i] - posOfLastPlacedCow >= distance)
            {
                posOfLastPlacedCow = stalls[i];
                cowsLeft--;

                if (cowsLeft == 0)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void resolve()
    {
        createValidStalls();
        sort(stalls.begin(), stalls.end());

        int start = 0;
        int end = stalls[n - 1] - stalls[0];
        int maxMinDistance = -1;

        while (start <= end)
        {
            int mid = start + (end - start) / 2;

            if (isValidDistance(mid))
            {
                maxMinDistance = mid;
                start = mid + 1;
            }
            else
            {
                end = mid - 1;
            }
        }

        cout << maxMinDistance << endl;
    }
};

int main()
{
    int t;

    cin >> t; // Number of test cases

    while (t > 0)
    {
        int n;
        int c;
        cin >> n >> c;

        Barn barn(n, c);

        barn.resolve();

        t--;
    }

    return 0;
}
