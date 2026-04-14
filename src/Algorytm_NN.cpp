#include "Algorytm_NN.h"
#include <vector>

using namespace std;

int rozwiazAlgorytm_NN(const Macierz& macierz) {
    int n = macierz.rozmiar;
    if (n <= 1) return 0;

    vector<bool> odwiedzone(n, false);
    int obecny = 0;
    odwiedzone[obecny] = true;
    
    int kosztCalkowity = 0;
    int odwiedzonych = 1;

    while (odwiedzonych < n) {
        int minKoszt = INF; 
        int nastepny = -1;

        for (int i = 0; i < n; i++) {
            if (odwiedzone[i] == false && macierz.dane[obecny][i] != INF) {
                if (macierz.dane[obecny][i] < minKoszt) {
                    minKoszt = macierz.dane[obecny][i];
                    nastepny = i;
                }
            }
        }

        if (nastepny == -1) return INF;

        odwiedzone[nastepny] = true;
        kosztCalkowity += minKoszt;
        obecny = nastepny;
        odwiedzonych++;
    }

    if (macierz.dane[obecny][0] == INF) return INF;
    kosztCalkowity += macierz.dane[obecny][0]; 
    
    return kosztCalkowity;
}