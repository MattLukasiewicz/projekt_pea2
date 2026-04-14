#include "Generator_macierzy.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

namespace fs = filesystem;

static void zapiszMacierzDoPliku(const string& sciezka, const vector<vector<int>>& macierz, int rozmiar) {
    ofstream plik(sciezka);
    if (!plik.is_open()) {
        cerr << "Wykryto blad, nie mozna utworzyc pliku: " << sciezka << "\n";
        return;
    }

    plik << rozmiar << "\n";

    for (int i = 0; i < rozmiar; ++i) {
        for (int j = 0; j < rozmiar; ++j) {
            plik << macierz[i][j] << " ";
        }
        plik << "\n";
    }

    plik << "\nsum_min=0\n";
    plik.close();
}

void generujWszystkieMacierze(const Konfiguracja& konf) {
    if (konf.genRozmiarMin <= 0 || konf.genRozmiarMax < konf.genRozmiarMin) {
        cerr << "Bledny zakres rozmiarow generatora.\n";
        return;
    }

    if (konf.genIleNaRozmiar <= 0) {
        cerr << "Liczba instancji na rozmiar musi byc > 0.\n";
        return;
    }

    if (konf.genWagaMin > konf.genWagaMax) {
        cerr << "Bledny zakres wag generatora.\n";
        return;
    }
    bool generujTsp = (konf.genTyp == 0 || konf.genTyp == 1);
    bool generujAtsp = (konf.genTyp == 0 || konf.genTyp == 2);

    if (!generujTsp && !generujAtsp) {
        cerr << "GEN_TYP ma niepoprawna wartosc. Uzyj 0 (oba), 1 (tsp), 2 (atsp).\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dystrybucja(konf.genWagaMin, konf.genWagaMax);

    string bazaFolderu = konf.genFolderDanych;
    if (!bazaFolderu.empty() && bazaFolderu.back() != '/' && bazaFolderu.back() != '\\') {
        bazaFolderu += '/';
    }

    for (int rozmiar = konf.genRozmiarMin; rozmiar <= konf.genRozmiarMax; ++rozmiar) {
        int liczba_instancji = konf.genIleNaRozmiar;
        string folderTsp = bazaFolderu + "tsp/" + to_string(rozmiar) + "/";
        string folderAtsp = bazaFolderu + "atsp/" + to_string(rozmiar) + "/";

        if (generujTsp) {
            fs::create_directories(folderTsp);
        }
        if (generujAtsp) {
            fs::create_directories(folderAtsp);
        }

        for (int instancja = 1; instancja <= liczba_instancji; ++instancja) {
            vector<vector<int>> macierzSymetryczna(rozmiar, vector<int>(rozmiar));
            vector<vector<int>> macierzAsymetryczna(rozmiar, vector<int>(rozmiar));

            for (int i = 0; i < rozmiar; ++i) {
                for (int j = 0; j < rozmiar; ++j) {
                    if (i == j) {
                        macierzSymetryczna[i][j] = -1;
                        macierzAsymetryczna[i][j] = -1;
                    } else if (i < j) {
                        int waga_sym = dystrybucja(gen);
                        int waga_asym1 = dystrybucja(gen);
                        int waga_asym2 = dystrybucja(gen);

                        macierzSymetryczna[i][j] = waga_sym;
                        macierzSymetryczna[j][i] = waga_sym;

                        macierzAsymetryczna[i][j] = waga_asym1;
                        macierzAsymetryczna[j][i] = waga_asym2;
                    }
                }
            }

            string nazwa_pliku = "macierz_" + to_string(rozmiar) + "x" + to_string(rozmiar) + "_" + to_string(instancja) + ".txt";

            if (generujTsp) {
                string sciezkaSym = folderTsp + nazwa_pliku;
                zapiszMacierzDoPliku(sciezkaSym, macierzSymetryczna, rozmiar);
            }

            if (generujAtsp) {
                string sciezkaAsym = folderAtsp + nazwa_pliku;
                zapiszMacierzDoPliku(sciezkaAsym, macierzAsymetryczna, rozmiar);
            }
        }
    }

    cout << "Macierze wygenerowane\n";
}
