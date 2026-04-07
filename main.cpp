#include <bits/stdc++.h>
using namespace std;

struct FastInput {
    static const size_t BUFSIZE = 1 << 20; // 1MB buffer
    int idx = 0, sz = 0;
    char buf[BUFSIZE];
    inline char read() {
        if (idx >= sz) {
            sz = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (sz == 0) return 0;
        }
        return buf[idx++];
    }
    template <class T>
    bool nextInt(T &out) {
        char c = read();
        if (!c) return false;
        T sign = 1;
        while (c && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = read();
        if (!c) return false;
        if (c == '-') { sign = -1; c = read(); }
        T x = 0;
        for (; c >= '0' && c <= '9'; c = read()) x = x * 10 + (c - '0');
        out = x * sign;
        return true;
    }
};

struct DSU {
    vector<int> p, r;
    explicit DSU(int n = 0) { init(n); }
    void init(int n) { p.resize(n); r.assign(n, 0); iota(p.begin(), p.end(), 0); }
    int find(int x) {
        while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
        return x;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastInput in;
    int t;
    if (!in.nextInt(t)) return 0;
    string result;
    result.reserve(t * 4);
    for (int cs = 0; cs < t; ++cs) {
        int n; in.nextInt(n);
        vector<unsigned long long> A(n), B(n);
        vector<unsigned char> E(n);
        for (int i = 0; i < n; ++i) {
            in.nextInt(A[i]);
            in.nextInt(B[i]);
            int e; in.nextInt(e);
            E[i] = (unsigned char)e;
        }
        // Coordinate compression
        vector<unsigned long long> vals;
        vals.reserve((size_t)2 * n);
        vals.insert(vals.end(), A.begin(), A.end());
        vals.insert(vals.end(), B.begin(), B.end());
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int m = (int)vals.size();
        // Map to indices once
        vector<int> IA(n), IB(n);
        for (int i = 0; i < n; ++i) {
            IA[i] = (int)(lower_bound(vals.begin(), vals.end(), A[i]) - vals.begin());
            IB[i] = (int)(lower_bound(vals.begin(), vals.end(), B[i]) - vals.begin());
        }
        // Free large originals early
        A.clear(); A.shrink_to_fit();
        B.clear(); B.shrink_to_fit();

        DSU dsu(m);
        for (int i = 0; i < n; ++i) if (E[i] == 1) dsu.unite(IA[i], IB[i]);
        bool ok = true;
        for (int i = 0; i < n; ++i) if (E[i] == 0 && dsu.same(IA[i], IB[i])) { ok = false; break; }
        result += ok ? "YES\n" : "NO\n";
    }
    cout << result;
    return 0;
}
