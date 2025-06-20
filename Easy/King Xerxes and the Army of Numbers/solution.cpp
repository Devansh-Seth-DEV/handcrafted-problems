#include <bits/stdc++.h>
using namespace std;

long long solve(const long long x, const int n) {
    long long mul = (((long long) pow(10,n))-1)/9;
    return x*mul;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while(t--) {
        int x, n;
        cin >> x >> n;
        cout << solve(x, n) << "\n";
    }
    
    return 0;
}
