#include <iostream>
#include "Wczytywanie_konfiguracji.h"
#include "Generator_macierzy.h"
#include "Test_pojedynczy.h"
#include "Testy_Katalogow_Macierzy.h"

using namespace std;

int main() {
    cout << "Wczytywanie konfiguracji...\n";
    Konfiguracja konf = Wczytywanie_Konfiguracji::wczytajKonfiguracje("config.txt");

    if (konf.uzyjUB == 1) {
        cout << "Tryb UB: NN (UZYJ_UB=1)\n";
    } else if (konf.uzyjUB == 2) {
        cout << "Tryb UB: RNN (UZYJ_UB=2)\n";
    } else {
        cout << "Tryb UB: brak dodatkowego ograniczenia (UZYJ_UB=0)\n";
    }

    switch (konf.tryb) {
        case 0: {
            cout << "TRYB 0: Pojedyncza macierz.\n";
            wykonajTestPojedynczy(konf);
            break;
        }
        case 1: {
            cout << "TRYB 1: Testy masowe w katalogu.\n";
            uruchomTestyDlaKatalogu(konf);
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
            cerr << "Wystapil blad: nieznany tryb uruchomienia!\n";
            break;
    }

    return 0;
}