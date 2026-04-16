#pragma once
#include <string>

struct Konfiguracja {
    int tryb;             // 0: pojedynczy, 1: katalog, 2: TSPLIB, 3: generator
    int algorytm;         // 0: BFS, 1: DFS, 2: BestFirst
    std::string sciezka;
    int uzyjUB;           // 0: brak, 1: NN, 2: RNN
    int rozmiarN;
    int powtorzenia;
    int pokazPostep;      // 0: nie, 1: tak

    // Parametry generatora (tryb 3)
    std::string genFolderDanych; // np. dane/generowane/
    int genTyp;                  // 0: oba, 1: tsp (sym), 2: atsp (asym)
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