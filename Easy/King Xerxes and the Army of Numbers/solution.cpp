#include <iostream>
using namespace std;

long long mul[15] = {
  1LL,
  11LL,
  111LL,
  1111LL,
  11111LL,
  111111LL,
  1111111LL,
  11111111LL,
  111111111LL,
  1111111111LL,
  11111111111LL,
  111111111111LL,
  1111111111111LL,
  11111111111111LL,
  111111111111111LL
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while(t--) {
        int x, n;
        cin >> x >> n;
        int q = n/15, r = n%15;
        while(q--) cout << ((long long)x)*mul[14];
        if (r) cout << ((long long)x)*mul[r-1];
        cout << "\n";
    }
    
    return 0;
}
