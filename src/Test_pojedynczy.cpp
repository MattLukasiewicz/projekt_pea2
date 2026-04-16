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

static double obliczPamiecMB(size_t maxWezlow) {
    const double bajtyNaMB = 1024.0 * 1024.0;
    return (static_cast<double>(maxWezlow) * static_cast<double>(sizeof(Wezel))) / bajtyNaMB;
}

static void zapiszWynikDoCsv(const Konfiguracja& konf, const string& sciezkaInstancji, int rozmiar, int wynik, double czasMs, size_t maxWezlow, double pamiecMB) {
    fs::create_directories("wyniki");

    string sciezkaPliku = "wyniki/Wyniki_pojedyncze.csv";
    bool nowyPlik = !fs::exists(sciezkaPliku);

    ofstream plik(sciezkaPliku, ios::app);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku wynikow: " << sciezkaPliku << "\n";
        return;
    }

    if (nowyPlik) {
        plik << "Plik;Rozmiar;Algorytm;Wynik;Czas_ms;Max_Wezlow;Pamiec_MB\n";
    }

    string nazwaPliku = fs::path(sciezkaInstancji).filename().string();
    if (nazwaPliku.empty()) {
        nazwaPliku = sciezkaInstancji;
    }

    plik << fixed << setprecision(3)
         << nazwaPliku << ";"
         << rozmiar << ";"
         << pobierzNazweAlgorytmu(konf.algorytm) << ";"
         << wynik << ";"
            << czasMs << ";"
            << maxWezlow << ";"
         << setprecision(6) << pamiecMB << "\n";
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

    WynikBnB wynikBnB{-1, 0};
    double czasMs = 0.0;

    if (konf.algorytm == 0) {
        Stoper stoper;
        stoper.start();
        wynikBnB = rozwiazAlgorytm_BnB_BFS(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else if (konf.algorytm == 1) {
        Stoper stoper;
        stoper.start();
        wynikBnB = rozwiazAlgorytm_BnB_DFS(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else if (konf.algorytm == 2) {
        Stoper stoper;
        stoper.start();
        wynikBnB = rozwiazAlgorytm_BnB_BestFirst(macierz, poczatkoweUB);
        stoper.stop();
        czasMs = stoper.pobierzCzasMs();
    } else {
        cerr << "Wystapil blad: nieznany algorytm!\n";
        return false;
    }

    cout << "Minimalny koszt: " << wynikBnB.koszt << "\n";
    cout << "Czas obliczen: " << czasMs << " ms\n";
    double pamiecMB = obliczPamiecMB(wynikBnB.maxWezlow);
    cout << "Max wezlow w kontenerze: " << wynikBnB.maxWezlow << "\n";
    cout << "Szacowana pamiec kontenera: " << fixed << setprecision(6) << pamiecMB << " MB\n";

    zapiszWynikDoCsv(konf, konf.sciezka, macierz.rozmiar, wynikBnB.koszt, czasMs, wynikBnB.maxWezlow, pamiecMB);

    return true;
}
