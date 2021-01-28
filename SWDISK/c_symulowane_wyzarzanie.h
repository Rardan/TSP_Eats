#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

class c_symulowane_wyzarzanie
{
public:
	double dlugosc_najlepszej_trasy;
	std::vector<int> najlepsza_trasa;

	c_symulowane_wyzarzanie(std::vector<std::vector<double>> _macierz_odleglosci, double _temperatura_poczatkowa, double _temperatura_chlodzenia, double _temperatura_minimalna, int _liczba_iteracji, bool _wyswietlanie = false);
	void znajdz_rozwiazanie();
	double dlugosc_trasy(std::vector<int> trasa);

private:
	std::vector<std::vector<double>> macierz_odleglosci;
	bool wyswietlanie;

	std::vector<int> obecna_trasa;
	double dlugosc_obecnej_trasy;

	double obecna_temperatura;
	double temperatura_chlodzenia;
	double temperatura_minimalna;
	int liczba_iteracji;

	void losowa_zamiana();
	double prawdopodobienstwo();
};