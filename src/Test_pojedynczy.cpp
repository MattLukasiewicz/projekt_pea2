#include "Test_pojedynczy.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "Wczytywanie_macierzy.h"
#include "Algorytm_BnB.h"
#include "Algorytm_NN.h"
#include "Stoper.h"

using namespace std;
namespace fs = filesystem;

static string pobierzNazweAlgorytmu(int algorytm) {
    switch (algorytm) {
        case 0: return "BFS";
        case 1: return "DFS";
        case 2: return "BestFirst";
        default: return "Nieznany";
    }
}

static int uruchomWybranyAlgorytmBnB(const Konfiguracja& konf, const Macierz& macierz, int poczatkoweUB) {
    switch (konf.algorytm) {
        case 0: return rozwiazAlgorytm_BnB_BFS(macierz, poczatkoweUB);
        case 1: return rozwiazAlgorytm_BnB_DFS(macierz, poczatkoweUB);
        case 2: return rozwiazAlgorytm_BnB_BestFirst(macierz, poczatkoweUB);
        default: return -1;
    }
}

static void zapiszWynikDoCsv(const Konfiguracja& konf, const string& sciezkaInstancji, int rozmiar, double czasMs) {
    fs::create_directories("wyniki");

    string sciezkaPliku = "wyniki/Wyniki_pojedyncze.csv";
    bool nowyPlik = !fs::exists(sciezkaPliku);

    ofstream plik(sciezkaPliku, ios::app);
    if (!plik.is_open()) {
        cerr << "Blad: nie mozna otworzyc pliku wynikow: " << sciezkaPliku << "\n";
        return;
    }

    if (nowyPlik) {
        plik << "NazwaPliku;Rozmiar;Algorytm;Czas_ms\n";
    }

    string nazwaPliku = fs::path(sciezkaInstancji).filename().string();
    if (nazwaPliku.empty()) {
        nazwaPliku = sciezkaInstancji;
    }

    plik << fixed << setprecision(3)
         << nazwaPliku << ";"
         << rozmiar << ";"
         << pobierzNazweAlgorytmu(konf.algorytm) << ";"
         << czasMs << "\n";
}

bool wykonajTestPojedynczy(const Konfiguracja& konf) {
    Macierz macierz = Wczytywanie_Macierzy::wczytajMacierz(konf.sciezka);
    if (macierz.rozmiar == 0) {
        cerr << "Blad: nie udalo sie wczytac macierzy z: " << konf.sciezka << "\n";
        return false;
    }

    int poczatkoweUB = INF;
    if (konf.uzyjUB == 1) {
        poczatkoweUB = rozwiazAlgorytm_NN(macierz);
    }

    Stoper stoper;
    stoper.start();
    int wynik = uruchomWybranyAlgorytmBnB(konf, macierz, poczatkoweUB);
    stoper.stop();

    if (wynik == -1) {
        cerr << "Blad: Nieznany algorytm!\n";
        return false;
    }

    double czasMs = stoper.pobierzCzasMs();

    cout << "Wyliczony przez B&B minimalny koszt: " << wynik << "\n";
    cout << "Czas obliczen: " << czasMs << " ms\n";

    zapiszWynikDoCsv(konf, konf.sciezka, macierz.rozmiar, czasMs);
    cout << "Wynik zapisany do: wyniki/Wyniki_pojedyncze.csv\n";

    return true;
}
