#include "Testy_Katalogow_Macierzy.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Algorytm_BnB.h"
#include "Algorytm_NN_i_RNN.h"
#include "Pasek_postepu.h"
#include "Stoper.h"
#include "Wczytywanie_macierzy.h"

using namespace std;
namespace fs = filesystem;

void uruchomTestyDlaKatalogu(const Konfiguracja& konf) {
    fs::path katalog = konf.sciezka;
    if (!fs::exists(katalog) || !fs::is_directory(katalog)) {
        cerr << "Wystapil blad: podana sciezka nie jest katalogiem: " << konf.sciezka << "\n";
        return;
    }

    long long laczniePlikow = 0;
    for (const auto& wpis : fs::directory_iterator(katalog)) {
        if (wpis.is_regular_file()) {
            ++laczniePlikow;
        }
    }
    long long wykonanePliki = 0;

    string nazwaAlgorytmu = "Nieznany";
    if (konf.algorytm == 0) nazwaAlgorytmu = "BFS";
    else if (konf.algorytm == 1) nazwaAlgorytmu = "DFS";
    else if (konf.algorytm == 2) nazwaAlgorytmu = "BestFirst";

    string typUB = "NIEZNANY";
    if (konf.uzyjUB == 0) typUB = "BRAK";
    else if (konf.uzyjUB == 1) typUB = "NN";
    else if (konf.uzyjUB == 2) typUB = "RNN";

    auto wyznaczPoczatkoweUB = [&](const Macierz& macierz) {
        if (konf.uzyjUB == 1) return algorytm_nn(macierz);
        if (konf.uzyjUB == 2) return algorytm_rnn(macierz);
        return INF;
    };

    if (konf.algorytm != 0 && konf.algorytm != 1 && konf.algorytm != 2) {
        cerr << "Wystapil blad: nieznany algorytm w konfiguracji.\n";
        return;
    }

    double sumaCzasow = 0.0;
    int iloscInstancji = 0;
    int rozmiarInstancji = konf.rozmiarN;

    pokazPostep(konf.pokazPostep == 1, wykonanePliki, laczniePlikow, "Testowanie macierzy");

    for (const auto& wpis : fs::directory_iterator(katalog)) {
        if (!wpis.is_regular_file()) {
            continue;
        }

        fs::path sciezkaPliku = wpis.path();
        Macierz macierz = Wczytywanie_Macierzy::wczytajMacierz(sciezkaPliku.string());
        if (macierz.rozmiar == 0) {
            cerr << "Wystapil blad: nie udalo sie wczytac macierzy z: " << sciezkaPliku.string() << "\n";
            wykonanePliki++;
            pokazPostep(konf.pokazPostep == 1, wykonanePliki, laczniePlikow, "Testowanie macierzy");
            continue;
        }

        rozmiarInstancji = macierz.rozmiar;

        int poczatkoweUB = wyznaczPoczatkoweUB(macierz);

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
        } else {
            Stoper stoper;
            stoper.start();
            wynik = rozwiazAlgorytm_BnB_BestFirst(macierz, poczatkoweUB);
            stoper.stop();
            czasMs = stoper.pobierzCzasMs();
        }

        wykonanePliki++;
        pokazPostep(konf.pokazPostep == 1, wykonanePliki, laczniePlikow, "Testowanie macierzy");

        sumaCzasow += czasMs;
        iloscInstancji++;

        cout << "Macierz: " << sciezkaPliku.filename().string()
             << " | koszt: " << wynik
             << " | czas: " << fixed << setprecision(3) << czasMs << " ms\n";
    }

    if (iloscInstancji == 0) {
        cerr << "Brak poprawnie przetworzonych instancji w katalogu: " << konf.sciezka << "\n";
        return;
    }

    double sredniCzas = sumaCzasow / iloscInstancji;

    fs::create_directories("wyniki");
    string sciezkaCsv = "wyniki/wyniki_testy_generowane_macierze.csv";
    bool nowyPlik = !fs::exists(sciezkaCsv);

    ofstream plikCsv(sciezkaCsv, ios::app);
    if (!plikCsv.is_open()) {
        cerr << "Wystapil blad: nie mozna otworzyc pliku CSV: " << sciezkaCsv << "\n";
        return;
    }

    if (nowyPlik) {
        plikCsv << "Rozmiar;Algorytm;Typ_UB;LiczbaInstancji;SredniCzas_ms\n";
    }

    plikCsv << fixed << setprecision(3)
            << rozmiarInstancji << ";"
            << nazwaAlgorytmu << ";"
            << typUB << ";"
            << iloscInstancji << ";"
            << sredniCzas << "\n";

    cout << "\n=== Podsumowanie testu ===\n";
    cout << "Rozmiar: " << rozmiarInstancji << "\n";
    cout << "Algorytm: " << nazwaAlgorytmu << "\n";
    cout << "Typ UB: " << typUB << "\n";
    cout << "Liczba instancji: " << iloscInstancji << "\n";
    cout << "Sredni czas: " << fixed << setprecision(3) << sredniCzas << " ms\n";
    cout << "Wynik dopisano do: " << sciezkaCsv << "\n";
}
