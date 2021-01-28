#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class c_przeglad_zupelny
{
public:
	double dlugosc_najlepszej_trasy;
	std::vector<int> najlepsza_trasa;

	c_przeglad_zupelny(std::vector<std::vector<double>> _macierz_odleglosci, bool _wyswietlanie = false);
	void znajdz_rozwiazanie(int pierwsza_restauracja);
	double oblicz_dlugosc_trasy(std::vector<int> trasa, int pierwsza_restauracja);
private:
	std::vector<std::vector<double>> macierz_odleglosci;
	bool wyswietlanie;
};