#include "c_przeglad_zupelny.h"

c_przeglad_zupelny::c_przeglad_zupelny(std::vector<std::vector<double>> _macierz_odleglosci, bool _wyswietlanie)
{
	macierz_odleglosci = _macierz_odleglosci;
	wyswietlanie = _wyswietlanie;

	dlugosc_najlepszej_trasy = INFINITY;
}

void c_przeglad_zupelny::znajdz_rozwiazanie(int pierwsza_restuaracja)
{
	double dlugosc_obecnej_trasy;
	std::vector<int> obecna_trasa;

	for (unsigned int i = 1; i < macierz_odleglosci.size(); i++)
		obecna_trasa.push_back(i);

	do {
		dlugosc_obecnej_trasy = oblicz_dlugosc_trasy(obecna_trasa, pierwsza_restuaracja);
		if (dlugosc_obecnej_trasy < dlugosc_najlepszej_trasy)
		{
			dlugosc_najlepszej_trasy = dlugosc_obecnej_trasy;
			najlepsza_trasa = obecna_trasa;
		}

		if (wyswietlanie)
		{
			std::cout << "Testowana trasa: "<< pierwsza_restuaracja << "-> ";
			for (unsigned int i = 0; i < obecna_trasa.size(); i++)
				std::cout << obecna_trasa[i] << "-> ";
			std::cout << pierwsza_restuaracja << std::endl;
			std::cout << "Jej dlugosc: " << dlugosc_obecnej_trasy << std::endl;
			std::cout << std::endl;
		}
	} while (std::next_permutation(obecna_trasa.begin(), obecna_trasa.end()));
}

double c_przeglad_zupelny::oblicz_dlugosc_trasy(std::vector<int> trasa, int pierwsza_restauracja)
{
	double dlugosc = 0;
	int poprzedni_element = pierwsza_restauracja;
	for (unsigned int i = 0; i < trasa.size(); i++)
	{
		dlugosc += macierz_odleglosci[poprzedni_element][trasa[i]];
		poprzedni_element = trasa[i];
	}
	dlugosc += macierz_odleglosci[poprzedni_element][0];
	return dlugosc;
}