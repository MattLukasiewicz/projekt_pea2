#pragma once
#include "Macierze.h"
#include <cstddef>
#include <vector>

struct Wezel {
    Macierz zredukowanaMacierz;
    int koszt;
    int wierzcholek;
    int poziom;
    std::vector<int> sciezka;

    Wezel(const Macierz& mat, int k, int w, int p, const std::vector<int>& s)
        : zredukowanaMacierz(mat), koszt(k), wierzcholek(w), poziom(p), sciezka(s) {}
};

struct WynikBnB {
    int koszt;
    size_t maxWezlow;
};

WynikBnB rozwiazAlgorytm_BnB_BFS(const Macierz& macierz, int poczatkoweUB);
WynikBnB rozwiazAlgorytm_BnB_DFS(const Macierz& macierz, int poczatkoweUB);
WynikBnB rozwiazAlgorytm_BnB_BestFirst(const Macierz& macierz, int poczatkoweUB);