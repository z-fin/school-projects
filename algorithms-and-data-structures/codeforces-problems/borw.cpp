/*
/
/   You have a sequence of integers. You can paint each of the integers black or white, or leave it unpainted.
/   The black integers must appear in ascending order and the white integers must appear in descending order.
/   The ascending/descending order must be strict, that is, two integers painted with the same color cannot be equal.
/   Paint the sequence so as to minimize the number of unpainted integers.
/
/   Submission: https://www.spoj.com/TDA242C1/files/src/33521936/
/
*/

#include <iostream>
#include <vector>
     
using namespace std;
     
#define MAX 1e9
     
vector<vector<vector<int>>> m;
vector<int> seq(200);
int n;
     
int resolve(int currIndex, int lastBlackIndex, int lastWhiteIndex) {
    if (currIndex >= n) {
            return 0;
        }
 
    if (m[currIndex][lastBlackIndex + 1][lastWhiteIndex + 1] != -1) {
        return m[currIndex][lastBlackIndex + 1][lastWhiteIndex + 1];
    }
 
    int leaveUnpainted = 1 + resolve(currIndex + 1, lastBlackIndex, lastWhiteIndex);
 
    int paintBlack = MAX;
    if (lastBlackIndex == -1 || seq[currIndex] > seq[lastBlackIndex]) {
        paintBlack = resolve(currIndex + 1, currIndex, lastWhiteIndex);
    }
 
    int paintWhite = MAX;
    if (lastWhiteIndex == -1 || seq[currIndex] < seq[lastWhiteIndex]) {
        paintWhite = resolve(currIndex + 1, lastBlackIndex, currIndex);
    }
 
    m[currIndex][lastBlackIndex + 1][lastWhiteIndex + 1] = min(leaveUnpainted, min(paintBlack, paintWhite));
    
    return m[currIndex][lastBlackIndex + 1][lastWhiteIndex + 1];
}
 
int main() {
    while (cin >> n) {
        if (n == -1) {
            return 0;
        }
        seq.clear();
        m.assign(n, vector<vector<int>>(n + 1, vector<int>(n + 1, -1)));
 
        for (int i = 0; i < n; ++i) {
            cin >> seq[i];
        }
 
        cout << resolve(0, -1, -1) << endl; 
    }
 
    return 0;
} 