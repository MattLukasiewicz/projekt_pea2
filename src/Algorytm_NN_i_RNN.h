#pragma once
#include "Macierze.h"

// Funkcja wyznaczajaca poczatkowe gorne ograniczenie (NN)
int algorytm_nn(const Macierz& macierz);

// Funkcja wyznaczajaca poczatkowe gorne ograniczenie (RNN)
int algorytm_rnn(const Macierz& macierz);

// Kompatybilnosc z dotychczasowymi wywolaniami
int rozwiazAlgorytm_NN(const Macierz& macierz);