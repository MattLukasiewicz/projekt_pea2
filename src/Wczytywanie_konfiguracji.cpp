#include "Wczytywanie_konfiguracji.h"
#include <fstream>
#include <iostream>

using namespace std;

Konfiguracja Wczytywanie_Konfiguracji::wczytajKonfiguracje(const string& nazwaPliku) {
    Konfiguracja konf{};
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: wczytywanie pliku konfiguracyjnego: " << nazwaPliku << "\n";
        return konf;
    }

    string linia;
    while (getline(plik, linia)) {
        size_t poz = linia.find('=');
        if (poz != string::npos) {
            string klucz = linia.substr(0, poz);
            string wartosc = linia.substr(poz + 1);
            if (!wartosc.empty() && wartosc.back() == '\r') wartosc.pop_back();

            if (klucz == "TRYB") konf.tryb = stoi(wartosc);
            else if (klucz == "ALGORYTM") konf.algorytm = stoi(wartosc);
            else if (klucz == "SCIEZKA_DANYCH") konf.sciezka = wartosc;
            else if (klucz == "UZYJ_UB") konf.uzyjUB = stoi(wartosc);
            else if (klucz == "POKAZ_POSTEP") konf.pokazPostep = stoi(wartosc);
            else if (klucz == "GEN_KATALOG_WYJSCIOWY") konf.genFolderDanych = wartosc;
            else if (klucz == "GEN_TYP") konf.genTyp = stoi(wartosc);
            else if (klucz == "GEN_ROZMIAR_MIN") konf.genRozmiarMin = stoi(wartosc);
            else if (klucz == "GEN_ROZMIAR_MAX") konf.genRozmiarMax = stoi(wartosc);
            else if (klucz == "GEN_INSTANCJE_NA_ROZMIAR") konf.genIleNaRozmiar = stoi(wartosc);
            else if (klucz == "GEN_WAGA_MIN") konf.genWagaMin = stoi(wartosc);
            else if (klucz == "GEN_WAGA_MAX") konf.genWagaMax = stoi(wartosc);
        }
    }

    return konf;
}