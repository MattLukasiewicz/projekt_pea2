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

static void zapiszWynikDoCsv(const Konfiguracja& konf, const string& sciezkaInstancji, int rozmiar, int wynik, double czasMs, double szacowanaPamiecMB) {
    fs::create_directories("wyniki");

    string sciezkaPliku = "wyniki/Wyniki_pojedyncze.csv";
    bool nowyPlik = !fs::exists(sciezkaPliku);

    ofstream plik(sciezkaPliku, ios::app);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku wynikow: " << sciezkaPliku << "\n";
        return;
    }

    if (nowyPlik) {
        plik << "Plik;Rozmiar;Algorytm;Wynik;Czas_ms;Szacowana_Pamiec_MB\n";
    }

    string nazwaPliku = fs::path(sciezkaInstancji).filename().string();
    if (nazwaPliku.empty()) {
        nazwaPliku = sciezkaInstancji;
    }

        plik << fixed << setprecision(4)
         << nazwaPliku << ";"
         << rozmiar << ";"
         << pobierzNazweAlgorytmu(konf.algorytm) << ";"
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

    int poczatkoweUB = INF;
    if (konf.uzyjUB == 1) {
        poczatkoweUB = algorytm_nn(macierz);
        cout << "Poczatkowe UB z NN: " << poczatkoweUB << "\n";
    } else if (konf.uzyjUB == 2) {
        poczatkoweUB = algorytm_rnn(macierz);
        cout << "Poczatkowe UB z RNN: " << poczatkoweUB << "\n";
    }

    WynikBnB wynikBnB{-1, 0};

    if (konf.algorytm == 0) {
        wynikBnB = rozwiazAlgorytm_BnB_BFS(macierz, poczatkoweUB);
    } else if (konf.algorytm == 1) {
        wynikBnB = rozwiazAlgorytm_BnB_DFS(macierz, poczatkoweUB);
    } else if (konf.algorytm == 2) {
        wynikBnB = rozwiazAlgorytm_BnB_BestFirst(macierz, poczatkoweUB);
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

    zapiszWynikDoCsv(konf, konf.sciezka, macierz.rozmiar, wynikBnB.koszt, czasMs, szacowanaPamiecMB);

    return true;
}
