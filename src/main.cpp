#include <iostream>
#include "Wczytywanie_konfiguracji.h"
#include "Generator_macierzy.h"
#include "Test_pojedynczy.h"

using namespace std;

int main() {
    cout << "Wczytywanie konfiguracji...\n";
    Konfiguracja konf = Wczytywanie_Konfiguracji::wczytajKonfiguracje("config.txt");

    switch (konf.tryb) {
        case 0: {
            cout << "TRYB 0: Pojedyncza macierz.\n";
            wykonajTestPojedynczy(konf);
            break;
        }
        case 1: {
            cout << "TRYB 1: Testy masowe w katalogu.\n";
            wykonajTestPojedynczy(konf);
            break;
        }
        case 2: {
            cout << "TRYB 2: Pliki formatu TSPLIB.\n";
            wykonajTestPojedynczy(konf);
            break;
        }
        case 3: {
            cout << "TRYB 3: Generowanie macierzy testowych.\n";
            generujWszystkieMacierze(konf);
            break;
        }
        default:
            cerr << "Blad: Nieznany tryb uruchomienia!\n";
            break;
    }

    return 0;
}