#include "Algorytm_BnB.h"
#include <queue>
#include <iostream>
#include <vector>

using namespace std;

WynikBnB rozwiazAlgorytm_BnB_BFS(const Macierz& macierz, int poczatkoweUB) {

    int n = macierz.rozmiar;
    int gorneOgraniczenie = poczatkoweUB;
    size_t maxWezlow = 0;

    Macierz macierzKorzenia = macierz;
    int kosztKorzenia = macierzKorzenia.redukuj(); 
    vector<int> sciezkaKorzenia = {0};
    
    Wezel korzen(macierzKorzenia, kosztKorzenia, 0, 1, sciezkaKorzenia);
    
    queue<Wezel> kolejka;
    kolejka.push(korzen);

    while (!kolejka.empty()) {
        if (kolejka.size() > maxWezlow) {
            maxWezlow = kolejka.size();
        }

        Wezel obecny = kolejka.front();
        kolejka.pop();

        if (obecny.koszt >= gorneOgraniczenie) {
            continue;
        }

        for (int i = 0; i < n; i++) {
            if (obecny.zredukowanaMacierz.dane[obecny.wierzcholek][i] != INF) {
                
                Macierz macierzPotomka = obecny.zredukowanaMacierz;
                
                for (int j = 0; j < n; j++) {
                    macierzPotomka.dane[obecny.wierzcholek][j] = INF;
                    macierzPotomka.dane[j][i] = INF;
                }
                macierzPotomka.dane[i][0] = INF; 

                int kosztPotomka = obecny.koszt + obecny.zredukowanaMacierz.dane[obecny.wierzcholek][i];
                kosztPotomka += macierzPotomka.redukuj(); 
                
                vector<int> sciezkaPotomka = obecny.sciezka;
                sciezkaPotomka.push_back(i);

                Wezel potomek(macierzPotomka, kosztPotomka, i, obecny.poziom + 1, sciezkaPotomka);

                if (potomek.poziom == n) {
                    if (potomek.koszt < gorneOgraniczenie) gorneOgraniczenie = potomek.koszt;
                } 
                else if (potomek.koszt < gorneOgraniczenie) {
                    kolejka.push(potomek);
                }
            }
        }
    }

    return {gorneOgraniczenie, maxWezlow};
}