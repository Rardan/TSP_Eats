#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class c_algorytm_zachlanny
{
public:
	double dlugosc_najlepszej_trasy;
	std::vector<int> najlepsza_trasa;

	c_algorytm_zachlanny(std::vector<std::vector<double>> _macierz_odleglosci, bool _wyswietlanie = false);
	void znajdz_rozwiazanie(int resturacja, std::vector<int> zamowienia);
	int ustal_nastepne_miasto(int poprzednie_miasto, std::vector<int> trasa, std::vector<int> zamowienia);
private:
	std::vector<std::vector<double>> macierz_odleglosci;
	bool wyswietlanie;
};