#include "Windows.h"
#include "wspolrzedne.h"
#include "c_przeglad_zupelny.h"
#include "c_symulowane_wyzarzanie.h"
#include "c_algorytm_zachlanny.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

enum Tsp { przeglad, wyzarzanie, zachlanny };

void testy();
std::vector<std::vector<double>> generuj_macierz_do_testow(int);
void dostarcz_zamowienia(std::vector<std::vector<double>>, bool, Tsp);
int wyznacz_restauracje(std::vector<std::vector<double>> macierz_odleglosci);
std::vector<std::vector<double>> wczytaj_macierz_odleglosci(std::string sciezka_dostepu);
double oblicz_odleglosc(wspolrzedne pkt1, wspolrzedne pkt2);
std::vector<int> znajdz_zamowienia_w_okolicy(std::vector<std::vector<double>>, std::vector<bool>, int, int);
double daj_wage(int, int, std::vector<std::vector<double>>);
std::vector<std::vector<double>> generuj_macierz_wybranych_zamowien(std::vector<std::vector<double>>, std::vector<int>);

bool sort_by_sec(const std::pair<int, double>& a, const std::pair<int, double>& b);

double PCFreq, CounterStart;
void StartCounter();
double GetCounter();

int main()
{
	std::string path = "test.tsp";
	std::vector<std::vector<double>> macierz_odleglosci_global = wczytaj_macierz_odleglosci(path);


	srand(time(NULL));
	std::cout << "Wybierz opcje:" << std::endl;
	std::cout << "1. Przeglad zupelny" << std::endl;
	std::cout << "2. Symulowane wyzarzanie" << std::endl;
	std::cout << "3. Algorytm zachlanny" << std::endl;
	std::cout << "4. Testy" << std::endl;
	std::cout << std::endl;

	int nr_opcji = 0;
	while (nr_opcji < 1 || nr_opcji > 4)
	{
		std::cout << "Podaj numer opcji: ";
		std::cin >> nr_opcji;
	}

	std::cout << std::endl;

	switch (nr_opcji)
	{
	case 1:
		dostarcz_zamowienia(macierz_odleglosci_global, false, Tsp::przeglad);
		break;
	case 2:
		dostarcz_zamowienia(macierz_odleglosci_global, false, Tsp::wyzarzanie);
		break;
	case 3:
		dostarcz_zamowienia(macierz_odleglosci_global, false, Tsp::zachlanny);
		break;
	case 4:
		testy();
		break;
	}

	system("pause");
	return 0;
}

void dostarcz_zamowienia(std::vector<std::vector<double>> macierz_odleglosci_global, bool test, Tsp algorytm)
{
	std::vector<std::vector<double>> macierz_odleglosci = macierz_odleglosci_global;

	std::vector<bool> zamowienia;

	for (int i = 0; i < macierz_odleglosci.size(); i++)
	{
		zamowienia.push_back(false);
	}

	if (!test)
	{
		std::cout << "Liczba wczytanych miast: " << macierz_odleglosci.size() << std::endl;
		std::cout << std::endl;
	}

	/*for (int i = 0; i < dostarczone.size(); i++)
	{
		for (int j = 0; j < dostarczone.size(); j++)
		{
			std::cout << dostarczone.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}*/

	int restauracja = wyznacz_restauracje(macierz_odleglosci);
	int restauracja2 = wyznacz_restauracje(macierz_odleglosci);
	while (restauracja == restauracja2)
		restauracja2 = wyznacz_restauracje(macierz_odleglosci);
	zamowienia.at(restauracja) = true;
	zamowienia.at(restauracja2) = true;

	if (macierz_odleglosci.size() < 2) return;

	std::string tmp;
	double temperatura_poczatkowa = 100000.0;
	double temperatura_chlodzenia = 0.9999;
	double temperatura_minimalna = 0.0001;
	int liczba_iteracji = (int)pow((double)macierz_odleglosci.size(), (double)2) / 4;

	if (!test && algorytm == Tsp::wyzarzanie)
	{
		std::cout << "Parametry proponowane:" << std::endl;
		std::cout << "Temperatura poczatkowa - 100000.0" << std::endl;
		std::cout << "Temperatura chlodzenia - 0.9999" << std::endl;
		std::cout << "Temperatura minimalna - 0.0001" << std::endl;
		std::cout << "Liczba iteracji dla poszczegolnych temperatur - n^2/4=" << liczba_iteracji << std::endl;
		std::cout << "Czy chcesz je zmienic? (tak/nie): ";
		std::cin >> tmp;

		std::cout << std::endl;

		if (tmp == "tak")
		{
			std::cout << "Temperatura poczatkowa: ";
			std::cin >> temperatura_poczatkowa;

			std::cout << "Temperatura chlodzenia: ";
			std::cin >> temperatura_chlodzenia;

			std::cout << "Temperatura minimalna: ";
			std::cin >> temperatura_minimalna;

			std::cout << "Liczba iteracji: ";
			std::cin >> liczba_iteracji;
		}

	}
	bool wszystko_dostarczone = false;

	while (!wszystko_dostarczone)
	{
		bool dostarczone_tmp = true;
		std::vector<int> vec = znajdz_zamowienia_w_okolicy(macierz_odleglosci, zamowienia, restauracja, restauracja2);

		std::vector<std::vector<double>> wybrane_zamowienia = generuj_macierz_wybranych_zamowien(macierz_odleglosci, vec);

		for (int i = 0; i < vec.size(); i++)
		{
			if (!test)
				std::cout << vec.at(i) << " ";

			zamowienia.at(vec.at(i)) = true;
		}
		if (!test)
			std::cout << std::endl;

		if (vec.size() > 2)
		{
			c_przeglad_zupelny przeglad_zupelny = c_przeglad_zupelny(wybrane_zamowienia, false);
			liczba_iteracji = (int)pow((double)wybrane_zamowienia.size(), (double)2) / 4;
			c_symulowane_wyzarzanie symulowane_wyzarzanie = c_symulowane_wyzarzanie(wybrane_zamowienia, temperatura_poczatkowa, temperatura_chlodzenia, temperatura_minimalna, liczba_iteracji, false);
			c_algorytm_zachlanny algorytm_zachlanny = c_algorytm_zachlanny(wybrane_zamowienia, false);
			std::vector<int> najlepsza_trasa;
			double dlugosc_najlepszej_trasy = 0.0;
			switch (algorytm)
			{
			case Tsp::przeglad:
				//c_przeglad_zupelny przeglad_zupelny = c_przeglad_zupelny(wybrane_zamowienia, false);
				przeglad_zupelny.znajdz_rozwiazanie(0);
				najlepsza_trasa = przeglad_zupelny.najlepsza_trasa;
				dlugosc_najlepszej_trasy = przeglad_zupelny.dlugosc_najlepszej_trasy;
				break;

			case Tsp::wyzarzanie:
				//c_symulowane_wyzarzanie symulowane_wyzarzanie = c_symulowane_wyzarzanie(wybrane_zamowienia, temperatura_poczatkowa, temperatura_chlodzenia, temperatura_minimalna, liczba_iteracji, false);
				symulowane_wyzarzanie.znajdz_rozwiazanie();
				najlepsza_trasa = symulowane_wyzarzanie.najlepsza_trasa;
				dlugosc_najlepszej_trasy = symulowane_wyzarzanie.dlugosc_najlepszej_trasy;
				break;

			case Tsp::zachlanny:
				algorytm_zachlanny.znajdz_rozwiazanie(0, vec);
				najlepsza_trasa = algorytm_zachlanny.najlepsza_trasa;
				dlugosc_najlepszej_trasy = algorytm_zachlanny.dlugosc_najlepszej_trasy;
				break;
			default:
				std::cout << "blad";
			}



			if (!test)
			{
				std::cout << "Najlepsza trasa: ";
				std::cout << vec.at(0) << "-> ";
				for (int i = 0; i < najlepsza_trasa.size(); i++)
					std::cout << vec.at(najlepsza_trasa[i]) << "-> ";
				std::cout << vec.at(0);
				std::cout << std::endl;
				std::cout << "Jej dlugosc: " << dlugosc_najlepszej_trasy << std::endl;
				std::cout << std::endl;
			}
		}
		else
		{
			if (!test)
			{
				std::cout << "Najlepsza trasa: ";
				for (int i = 0; i < vec.size(); i++)
					std::cout << vec.at(i) << "-> ";
				std::cout << vec.at(0);
				std::cout << std::endl;
			}
			double droga = 0.0;
			for (int i = 0; i < vec.size() - 1; i++)
				droga += daj_wage(vec.at(i), vec.at(i + 1), macierz_odleglosci);
			droga += daj_wage(vec.at(0), vec.at(vec.size() - 1), macierz_odleglosci);

			if (!test)
			{
				std::cout << "Jej dlugosc: " << droga << std::endl;
				std::cout << std::endl;
			}
		}


		for (int i = 0; i < zamowienia.size(); i++)
		{
			if (zamowienia.at(i) == false)
				dostarczone_tmp = false;
		}

		if (dostarczone_tmp)
			break;
	}
}

void testy()
{
	std::ofstream plik_przeg, plik_wyz, plik_zach;
	plik_przeg.open("wyniki_przeglad.txt");
	plik_wyz.open("wyniki_wyzarzanie.txt");
	plik_zach.open("wyniki_zachlanny.txt");
	std::vector<int> rozmiar = { 10, 20, 30, 50, 100, 150 };
	double powtorzenia = 20.0;



	for (int i = 0; i < rozmiar.size(); i++)
	{
		std::cout << rozmiar.at(i) << std::endl;
		double czas_przeg = 0.0, czas_wyz = 0.0, czas_zach = 0.0;
		std::vector< std::vector<double>> matrix = generuj_macierz_do_testow(rozmiar.at(i));
		for (int j = 0; j < (int)powtorzenia; j++)
		{
			StartCounter();
			dostarcz_zamowienia(matrix, true, Tsp::przeglad);
			czas_przeg += GetCounter();

			StartCounter();
			dostarcz_zamowienia(matrix, true, Tsp::wyzarzanie);
			czas_wyz += GetCounter();

			StartCounter();
			dostarcz_zamowienia(matrix, true, Tsp::zachlanny);
			czas_zach += GetCounter();
		}
		plik_przeg << rozmiar.at(i) << " " << czas_przeg / powtorzenia << "\n";
		plik_wyz << rozmiar.at(i) << " " << czas_wyz / powtorzenia << "\n";
		plik_zach << rozmiar.at(i) << " " << czas_zach / powtorzenia << "\n";
	}

	plik_przeg.close();
	plik_wyz.close();
	plik_zach.close();

	std::cout << "KONIEC!";
}

std::vector<std::vector<double>> wczytaj_macierz_odleglosci(std::string sciezka_dostepu)
{
	std::vector<std::vector<double>> macierz_odleglosci;

	std::ifstream plik;
	plik.open(sciezka_dostepu);

	if (plik.is_open())
	{
		std::string tmp, typ;
		int liczba_miast;

		while (tmp != "DIMENSION:" && tmp != "DIMENSION")
			plik >> tmp;
		if (tmp == "DIMENSION") plik >> tmp;
		plik >> liczba_miast;

		while (tmp != "EDGE_WEIGHT_TYPE:" && tmp != "EDGE_WEIGHT_TYPE")
			plik >> tmp;
		if (tmp == "EDGE_WEIGHT_TYPE") plik >> tmp;
		plik >> typ;

		if (typ == "EUC_2D" || typ == "GEO")
		{
			std::vector<wspolrzedne> wspolrzedne_miast;
			double x, y;

			while (tmp != "NODE_COORD_SECTION")
				plik >> tmp;

			for (int i = 0; i < liczba_miast; i++)
			{
				plik >> tmp >> x >> y;
				wspolrzedne_miast.push_back(wspolrzedne(x, y));
			}

			for (unsigned int i = 0; i < wspolrzedne_miast.size(); i++)
			{
				std::vector<double> wiersz_macierzy_odleglosci;
				macierz_odleglosci.push_back(wiersz_macierzy_odleglosci);

				for (unsigned int j = 0; j < wspolrzedne_miast.size(); j++)
				{
					double odleglosc = oblicz_odleglosc(wspolrzedne_miast[i], wspolrzedne_miast[j]);
					macierz_odleglosci[i].push_back(odleglosc);
				}
			}
		}
		else if (typ == "EXPLICIT")
		{
			double odleglosc;

			while (tmp != "EDGE_WEIGHT_SECTION")
				plik >> tmp;

			for (int i = 0; i < liczba_miast; i++)
			{
				std::vector<double> wiersz_macierzy_odleglosci;
				macierz_odleglosci.push_back(wiersz_macierzy_odleglosci);

				for (int j = 0; j < liczba_miast; j++)
				{
					plik >> odleglosc;
					macierz_odleglosci[i].push_back(odleglosc);
				}
			}
		}
	}

	return macierz_odleglosci;
}

int wyznacz_restauracje(std::vector<std::vector<double>> macierz_odleglosci)
{
	return (std::rand() % macierz_odleglosci.size()) + 0;
}

std::vector<int> znajdz_zamowienia_w_okolicy(std::vector<std::vector<double>> macierz_odleglosci, std::vector<bool> zamowienia, int restauracja, int restauracja2)
{
	int najstarsze = -1;
	int wybrana_restauracja = -1;
	double waga_ograniczona = DBL_MAX;
	for (int i = 0; i < zamowienia.size(); i++)
	{
		if (zamowienia.at(i) == false)
		{
			najstarsze = i;
			double droga_do_r = daj_wage(i, restauracja, macierz_odleglosci);
			double droga_do_r2 = daj_wage(i, restauracja2, macierz_odleglosci);
			if (droga_do_r >= droga_do_r2)
			{
				wybrana_restauracja = restauracja2;
				waga_ograniczona = droga_do_r2;
			}
			else
			{
				wybrana_restauracja = restauracja;
				waga_ograniczona = droga_do_r;
			}
			break;

		}
	}

	std::vector<std::pair<int, double>> zamowienia_spelniajace_wymagania;
	for (int i = 0; i < zamowienia.size(); i++)
	{
		if (zamowienia.at(i) == false && i != najstarsze)
		{
			double droga = daj_wage(i, najstarsze, macierz_odleglosci);
			if (droga <= waga_ograniczona)
			{
				std::pair<int, double> zmw;
				zmw.first = i;
				zmw.second = droga;
				zamowienia_spelniajace_wymagania.push_back(zmw);
			}
		}
	}

	std::sort(zamowienia_spelniajace_wymagania.begin(), zamowienia_spelniajace_wymagania.end(), sort_by_sec);

	/*for (int i = 0; i < zamowienia_spelniajace_wymagania.size(); i++)
	{
		std::cout << zamowienia_spelniajace_wymagania.at(i).first << " " << zamowienia_spelniajace_wymagania.at(i).second << std::endl;
	}*/

	std::vector<int> zamowienia_return;

	if (zamowienia_spelniajace_wymagania.size() > 4)
	{
		for (int i = 0; i < 4; i++)
			zamowienia_return.push_back(zamowienia_spelniajace_wymagania.at(i).first);
	}
	else
	{
		for (int i = 0; i < zamowienia_spelniajace_wymagania.size(); i++)
			zamowienia_return.push_back(zamowienia_spelniajace_wymagania.at(i).first);
	}

	zamowienia_return.push_back(najstarsze);

	std::vector<int> zamowienia_last;
	zamowienia_last.push_back(wybrana_restauracja);

	for (int i = 0; i < zamowienia_return.size(); i++)
		zamowienia_last.push_back(zamowienia_return.at(i));

	return zamowienia_last;
}

std::vector<std::vector<double>> generuj_macierz_wybranych_zamowien(std::vector<std::vector<double>> macierz_odleglosci, std::vector<int> zamowienia)
{
	std::vector<std::vector<double>> macierz_wybranych_zamowien;

	for (int i = 0; i < zamowienia.size(); i++)
	{
		std::vector<double> vec;
		for (int j = 0; j < zamowienia.size(); j++)
		{
			vec.push_back(macierz_odleglosci.at(zamowienia.at(i)).at(zamowienia.at(j)));
		}
		macierz_wybranych_zamowien.push_back(vec);
	}

	/*std::cout << std::endl;

	for (int i = 0; i < macierz_wybranych_zamowien.size(); i++)
	{
		for (int j = 0; j < macierz_wybranych_zamowien.size(); j++)
		{
			std::cout << macierz_wybranych_zamowien.at(i).at(j) << " ";
		}
		std::cout << std::endl;
	}*/

	return macierz_wybranych_zamowien;
}

double oblicz_odleglosc(wspolrzedne pkt1, wspolrzedne pkt2)
{
	double delta_x = abs(pkt2.x - pkt1.x);
	double delta_y = abs(pkt2.y - pkt1.y);
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

double daj_wage(int x, int y, std::vector<std::vector<double>> macierz_odleglosci)
{
	return macierz_odleglosci.at(x).at(y);
}

bool sort_by_sec(const std::pair<int, double>& a, const std::pair<int, double>& b)
{
	return (a.second < b.second);
}

std::vector<std::vector<double>> generuj_macierz_do_testow(int n)
{
	srand(time(NULL));
	std::vector<std::vector<double>> matrix;
	for (int i = 0; i < n; i++)
	{
		std::vector<double> vec;
		for (int j = 0; j < n; j++)
		{
			vec.push_back(0.0);
		}
		matrix.push_back(vec);
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				matrix.at(i).at(j) = 0;
			else
			{
				int val = rand() % 500 + 50;
				matrix.at(i).at(j) = (double)val;
				matrix.at(j).at(i) = (double)val;
			}
		}
	}

	return matrix;
}

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = (double)li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}