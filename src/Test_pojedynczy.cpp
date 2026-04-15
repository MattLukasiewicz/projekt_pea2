#include "Test_pojedynczy.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "Wczytywanie_macierzy.h"
#include "Algorytm_BnB.h"
#include "Algorytm_NN_i_RNN.h"
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

static void zapiszWynikDoCsv(const Konfiguracja& konf, const string& sciezkaInstancji, int rozmiar, double czasMs) {
    fs::create_directories("wyniki");

    string sciezkaPliku = "wyniki/Wyniki_pojedyncze.csv";
    bool nowyPlik = !fs::exists(sciezkaPliku);

    ofstream plik(sciezkaPliku, ios::app);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku wynikow: " << sciezkaPliku << "\n";
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
        cerr << "Wystapil blad: nie udalo sie wczytac macierzy z: " << konf.sciezka << "\n";
        return false;
    }

    int poczatkoweUB = INF;
    if (konf.uzyjUB == 1) {
        poczatkoweUB = algorytm_nn(macierz);
        cout << "Poczatkowe UB z NN: " << poczatkoweUB << "\n";
    } else if (konf.uzyjUB == 2) {
        poczatkoweUB = algorytm_rnn(macierz);
        cout << "Poczatkowe UB z RNN: " << poczatkoweUB << "\n";
    }

    int wynik = -1;
    double czasMs = 0.0;

    if (konf.algorytm == 0) {
        Stoper stoper;
        stoper.start();
        wynik = rozwiazAlgorytm_BnB_BFS(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else if (konf.algorytm == 1) {
        Stoper stoper;
        stoper.start();
        wynik = rozwiazAlgorytm_BnB_DFS(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else if (konf.algorytm == 2) {
        Stoper stoper;
        stoper.start();
        wynik = rozwiazAlgorytm_BnB_BestFirst(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else {
        cerr << "Wystapil blad: nieznany algorytm!\n";
        return false;
    }

    cout << "Minimalny koszt: " << wynik << "\n";
    cout << "Czas obliczen: " << czasMs << " ms\n";

    zapiszWynikDoCsv(konf, konf.sciezka, macierz.rozmiar, czasMs);

    return true;
}
