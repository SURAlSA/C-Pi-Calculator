#include <cmath>
#include <iostream>
#include <fstream>
#include <gmpxx.h>

using namespace std;

struct PQT {
    mpz_class P, Q, T;
};

void computePI(int digits);
PQT computePQT(int n1, int n2);

int DIGITS;
mpz_class A = 13591409;
mpz_class B = 545140134;
mpz_class C = 640320; 
mpz_class D = 426880;
mpz_class E = 10005;
double DIGITS_per_term = 14.1816474627254776555; // log(53360^3) / log(10)
mpz_class C3_24 = C * C * C / 24;
clock_t c0, c1, c2;

int main() {
    try {
        cout << "How many Digits of Pi do you wish to calculate?: \n";
        cin >> DIGITS;

        if (cin.fail() || DIGITS <= 0) {
            throw invalid_argument("DIGITS must be a positive integer");
        }

        int PREC = DIGITS * log2(10);
        int N = DIGITS / DIGITS_per_term;
        
        computePI(DIGITS);
        
    } catch (...) {
        cout << "DIGITS must be an integer!\n";
        return -1;
    }

    return 0;
}

void computePI(int digits) {
    cout << "Calculating (" << digits << " PI digits)" << endl;

    c0 = clock();

    int PREC = digits * log2(10);
    int N = digits / DIGITS_per_term;

    PQT PQT = computePQT(0, N);
    mpf_class pi(0, PREC);
    pi  = D * sqrt((mpf_class)E) * PQT.Q;
    pi /= (A * PQT.Q + PQT.T);

    c1 = clock();
    cout << "Time to Compute: " << (double)(c1 - c0) / CLOCKS_PER_SEC
         << " Seconds." << endl;

    ofstream ofs ("PI.txt");
    ofs.precision(digits + 1);
    ofs << pi << endl;

    c2 = clock();
    cout << "Time to write: " << (double)(c2 - c1) / CLOCKS_PER_SEC 
         << " Seconds." << endl;
}

PQT computePQT(int n1, int n2) {
    int m;
    PQT res;

    if (n1 + 1 == n2) {
        res.P  = (2 * n2 - 1);
        res.P *= (6 * n2 - 1);
        res.P *= (6 * n2 - 5);
        res.Q  = C3_24 * n2 * n2 * n2;
        res.T  = (A + B * n2) * res.P;
        if ((n2 & 1) == 1) res.T = - res.T;
    } else {
        m = (n1 + n2) / 2;
        PQT res1 = computePQT(n1, m);
        PQT res2 = computePQT(m, n2);
        res.P = res1.P * res2.P;
        res.Q = res1.Q * res2.Q;
        res.T = res1.T * res2.Q + res1.P * res2.T;
    }

    return res;
}
