#pragma once
#include <string>

struct Konfiguracja {
    int tryb;
    int algorytm;
    std::string sciezka;
    int uzyjUB;
    int pokazPostep;

    std::string genFolderDanych;
    int genTyp;
    int genRozmiarMin;
    int genRozmiarMax;
    int genIleNaRozmiar;
    int genWagaMin;
    int genWagaMax;
};

class Wczytywanie_Konfiguracji {
public:
    static Konfiguracja wczytajKonfiguracje(const std::string& nazwaPliku);
};