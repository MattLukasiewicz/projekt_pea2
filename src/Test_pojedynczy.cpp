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

static string pobierzTypUB(int uzyjUB) {
    switch (uzyjUB) {
        case 0: return "BRAK";
        case 1: return "NN";
        case 2: return "RNN";
        default: return "NIEZNANY";
    }
}

static void zapiszWynikDoCsv(const Konfiguracja& konf, const string& sciezkaInstancji, int rozmiar, int wynik, int ub0, double czasMs, double szacowanaPamiecMB) {
    fs::create_directories("wyniki");

    string sciezkaPliku = "wyniki/Wyniki_pojedyncze.csv";
    bool nowyPlik = !fs::exists(sciezkaPliku);

    ofstream plik(sciezkaPliku, ios::app);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku wynikow: " << sciezkaPliku << "\n";
        return;
    }

    if (nowyPlik) {
        plik << "Plik;Rozmiar;Algorytm;Typ_UB;UB;Wynik;Czas_ms;Szacowana_Pamiec_MB\n";
    }

    string nazwaPliku = fs::path(sciezkaInstancji).filename().string();
    if (nazwaPliku.empty()) {
        nazwaPliku = sciezkaInstancji;
    }

        plik << fixed << setprecision(4)
         << nazwaPliku << ";"
         << rozmiar << ";"
         << pobierzNazweAlgorytmu(konf.algorytm) << ";"
            << pobierzTypUB(konf.uzyjUB) << ";"
            << (ub0 == INF ? string("BRAK") : to_string(ub0)) << ";"
         << wynik << ";"
            << czasMs << ";"
            << szacowanaPamiecMB << "\n";
}

bool wykonajTestPojedynczy(const Konfiguracja& konf) {
    Macierz macierz = Wczytywanie_Macierzy::wczytajMacierz(konf.sciezka);
    if (macierz.rozmiar == 0) {
        cerr << "Wystapil blad: nie udalo sie wczytac macierzy z: " << konf.sciezka << "\n";
        return false;
    }

    Stoper stoper;
    stoper.start();

    int ub0 = INF;
    if (konf.uzyjUB == 1) {
        ub0 = algorytm_nn(macierz);
        cout << "Poczatkowe UB z NN: " << ub0 << "\n";
    } else if (konf.uzyjUB == 2) {
        ub0 = algorytm_rnn(macierz);
        cout << "Poczatkowe UB z RNN: " << ub0 << "\n";
    }

    WynikBnB wynikBnB{-1, 0};

    if (konf.algorytm == 0) {
        wynikBnB = rozwiazAlgorytm_BnB_BFS(macierz, ub0);
    } else if (konf.algorytm == 1) {
        wynikBnB = rozwiazAlgorytm_BnB_DFS(macierz, ub0);
    } else if (konf.algorytm == 2) {
        wynikBnB = rozwiazAlgorytm_BnB_BestFirst(macierz, ub0);
    } else {
        stoper.stop();
        cerr << "Wystapil blad: nieznany algorytm!\n";
        return false;
    }

    stoper.stop();
    double czasMs = stoper.pobierzCzasMs();

    int N = macierz.rozmiar;
    size_t rozmiarWezlaBajty = sizeof(WynikBnB)
        + (static_cast<size_t>(N) * static_cast<size_t>(N) * sizeof(int))
        + (static_cast<size_t>(N) * sizeof(int));
    double szacowanaPamiecMB = (static_cast<double>(wynikBnB.maxWezlow) * static_cast<double>(rozmiarWezlaBajty))
        / (1024.0 * 1024.0);

    cout << "Minimalny koszt: " << wynikBnB.koszt << "\n";
    cout << "Czas obliczen: " << fixed << setprecision(4) << czasMs << " ms\n";
    cout << "Szacowana pamiec kontenera: " << fixed << setprecision(4) << szacowanaPamiecMB << " MB\n";

    zapiszWynikDoCsv(konf, konf.sciezka, macierz.rozmiar, wynikBnB.koszt, ub0, czasMs, szacowanaPamiecMB);

    return true;
}
