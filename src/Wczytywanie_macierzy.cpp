#include "Wczytywanie_macierzy.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

Macierz Wczytywanie_Macierzy::wczytajMacierz(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: wczytywanie macierzy: " << nazwaPliku << "\n";
        return Macierz(0);
    }

    string pierwszeSlowo;
    plik >> pierwszeSlowo;

    if (isdigit(pierwszeSlowo[0]) || (pierwszeSlowo[0] == '-' && pierwszeSlowo.size() > 1)) {
        int n = stoi(pierwszeSlowo);
        Macierz mat(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int wartosc;
                plik >> wartosc;
                if (i == j || wartosc < 0) mat.dane[i][j] = INF;
                else mat.dane[i][j] = wartosc;
            }
        }

        string temp;
        while (plik >> temp) {
            size_t poz = temp.find("sum_min=");
            if (poz != string::npos) {
                mat.optymalnyKoszt = stoi(temp.substr(poz + 8));
                break;
            }
        }
        return mat;
    }
    else {
        plik.clear();
        plik.seekg(0);

        int n = 0;
        string format = "FULL_MATRIX";
        bool wSekcji = false;
        string linia;

        while (getline(plik, linia)) {
            if (linia.find("DIMENSION") != string::npos) {
                size_t dwukropek = linia.find(':');
                if (dwukropek != string::npos) {
                    n = stoi(linia.substr(dwukropek + 1));
                }
            }
            else if (linia.find("EDGE_WEIGHT_FORMAT") != string::npos) {
                size_t dwukropek = linia.find(':');
                if (dwukropek != string::npos) {
                    format = linia.substr(dwukropek + 1);
                    format.erase(remove_if(format.begin(), format.end(), ::isspace), format.end());
                }
            }
            else if (linia.find("EDGE_WEIGHT_SECTION") != string::npos) {
                wSekcji = true;
                break;
            }
        }

        if (!wSekcji || n == 0) return Macierz(0);

        Macierz mat(n);
        if (format == "FULL_MATRIX") {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int wartosc;
                    plik >> wartosc;
                    if (i == j || wartosc >= 999999) mat.dane[i][j] = INF;
                    else mat.dane[i][j] = wartosc;
                }
            }
        }
        else if (format == "UPPER_ROW") {
            for (int i = 0; i < n - 1; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    int wartosc;
                    plik >> wartosc;
                    mat.dane[i][j] = wartosc;
                    mat.dane[j][i] = wartosc;
                }
            }
            for (int i = 0; i < n; ++i) mat.dane[i][i] = INF;
        }

        string temp;
        while (plik >> temp) {
            size_t poz = temp.find("sum_min=");
            if (poz != string::npos) {
                mat.optymalnyKoszt = stoi(temp.substr(poz + 8));
                break;
            }
        }
        return mat;
    }
}