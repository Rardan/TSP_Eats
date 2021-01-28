#include "c_symulowane_wyzarzanie.h"

// https://tdb0.wordpress.com/2010/12/03/symulowane-wyzarzanie/

c_symulowane_wyzarzanie::c_symulowane_wyzarzanie::c_symulowane_wyzarzanie(std::vector<std::vector<double>> _macierz_odleglosci, double _temperatura_poczatkowa, double _temperatura_chlodzenia, double _temperatura_minimalna, int _liczba_iteracji, bool _wyswietlanie)
{
	macierz_odleglosci = _macierz_odleglosci;
	wyswietlanie = _wyswietlanie;

	obecna_temperatura = _temperatura_poczatkowa;
	temperatura_chlodzenia = _temperatura_chlodzenia;
	temperatura_minimalna = _temperatura_minimalna;
	liczba_iteracji = _liczba_iteracji;

	for (unsigned int i = 1; i < macierz_odleglosci.size(); i++)
		obecna_trasa.push_back(i);
	dlugosc_obecnej_trasy = dlugosc_trasy(obecna_trasa);

	najlepsza_trasa = obecna_trasa;
	dlugosc_najlepszej_trasy = dlugosc_obecnej_trasy;

	srand((unsigned int)time(NULL));
}

void c_symulowane_wyzarzanie::znajdz_rozwiazanie()
{
	while (obecna_temperatura > temperatura_minimalna)
	{
		for (int j = 0; j < liczba_iteracji; j++)
		{
			losowa_zamiana();
			dlugosc_obecnej_trasy = dlugosc_trasy(obecna_trasa);

			if (dlugosc_obecnej_trasy < dlugosc_najlepszej_trasy
				|| ((double)rand() / (double)RAND_MAX) < prawdopodobienstwo())
			{
				najlepsza_trasa = obecna_trasa;
				dlugosc_najlepszej_trasy = dlugosc_obecnej_trasy;
			}

			if (wyswietlanie)
			{
				std::cout << "Testowana trasa: 0, ";
				for (unsigned int i = 0; i < obecna_trasa.size(); i++)
					std::cout << obecna_trasa[i] << ", ";
				std::cout << "0" << std::endl;
				std::cout << "Jej dlugosc: " << dlugosc_obecnej_trasy << std::endl;
				std::cout << std::endl;
			}
		}
		
		obecna_temperatura *= temperatura_chlodzenia;
		//std::cout << obecna_temperatura << std::endl;
	}
}

void c_symulowane_wyzarzanie::losowa_zamiana()
{
	int ilosc_miast = najlepsza_trasa.size();
	int a = std::rand() % ilosc_miast;
	int b = std::rand() % ilosc_miast;
	while (a == b) b = std::rand() % ilosc_miast;

	obecna_trasa = najlepsza_trasa;
	int tmp = obecna_trasa[a];
	obecna_trasa[a] = obecna_trasa[b];
	obecna_trasa[b] = tmp;
}

double c_symulowane_wyzarzanie::prawdopodobienstwo()
{
	double potega = -((dlugosc_obecnej_trasy - dlugosc_najlepszej_trasy) / obecna_temperatura);
	return pow(M_E, potega);
}

double c_symulowane_wyzarzanie::dlugosc_trasy(std::vector<int> trasa)
{
	double dlugosc = 0;
	int poprzedni_element = 0;
	for (unsigned int i = 0; i < trasa.size(); i++)
	{
		dlugosc += macierz_odleglosci[poprzedni_element][trasa[i]];
		poprzedni_element = trasa[i];
	}
	dlugosc += macierz_odleglosci[poprzedni_element][0];
	return dlugosc;
}