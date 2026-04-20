#Opis

To jest projekt w C++, ktory rozwiazuje TSP/ATSP algorytmami Branch and Bound.
W projekcie sa 3 warianty BnB: BFS, DFS i BestFirst.
Program moze korzystac z poczatkowego UB (NN albo RNN), a wyniki zapisuje do CSV.

Ustawienia techniczne projektu:
- jezyk: C++17
- kompilator: g++
- flagi kompilacji: -O2 -Wall -Wextra
- pliki zrodlowe: src/*.cpp

## Jak uruchomic

1. Wklej jeden z gotowych wariantow config do pliku config.txt.
2. Uruchom program:
   - .\pea2.exe
   lub
   - .\bin\pea2.exe
3. Odczytaj wyniki:
   - wyniki/Wyniki_pojedyncze.csv
   - wyniki/wyniki_testy_generowane_macierze.csv

## Wyjasnienie parametrow config

- TRYB:
  - 0 = pojedyncza macierz
  - 1 = test katalogu
  - 3 = generator (ponizej nie uzywamy)
- ALGORYTM:
  - 0 = BFS
  - 1 = DFS
  - 2 = BestFirst
- UZYJ_UB:
  - 0 = brak
  - 1 = NN
  - 2 = RNN
- SCIEZKA_DANYCH = plik lub katalog wejscia
- POKAZ_POSTEP = pokazuje pasek postepu dla testu katalogowego (TRYB=1)

## Gotowe konfiguracje do wklejenia

OPCJA A - TRYB 0, BFS, UB z NN, mala macierz

TRYB=0
ALGORYTM=0
SCIEZKA_DANYCH=dane/macierze_testowe/matrix_6x6.atsp
UZYJ_UB=1
POKAZ_POSTEP=0

OPCJA B - TRYB 0, DFS, UB z RNN

TRYB=0
ALGORYTM=1
SCIEZKA_DANYCH=dane/macierze_testowe/matrix_8x8.atsp
UZYJ_UB=2
POKAZ_POSTEP=0

OPCJA C - TRYB 0, BestFirst, bez UB

TRYB=0
ALGORYTM=2
SCIEZKA_DANYCH=dane/macierze_testowe/matrix_11x11.atsp
UZYJ_UB=0
POKAZ_POSTEP=0

OPCJA D - TRYB 1, katalog testowy, BestFirst, UB z NN

TRYB=1
ALGORYTM=2
SCIEZKA_DANYCH=dane/macierze_testowe
UZYJ_UB=1
POKAZ_POSTEP=1
