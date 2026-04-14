#pragma once
#include "Macierze.h"
#include <vector>

// Polska struktura wezla uzywana w drzewie przeszukiwan B&B
struct Wezel {
    Macierz zredukowanaMacierz;
    int koszt;
    int wierzcholek;
    int poziom;
    std::vector<int> sciezka;

    // Konstruktor
    Wezel(const Macierz& mat, int k, int w, int p, const std::vector<int>& s)
        : zredukowanaMacierz(mat), koszt(k), wierzcholek(w), poziom(p), sciezka(s) {}
};

// Deklaracje funkcji przeszukujacych
int rozwiazAlgorytm_BnB_BFS(const Macierz& macierz, int poczatkoweUB);
int rozwiazAlgorytm_BnB_DFS(const Macierz& macierz, int poczatkoweUB);
int rozwiazAlgorytm_BnB_BestFirst(const Macierz& macierz, int poczatkoweUB);