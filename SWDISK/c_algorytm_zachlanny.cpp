#include "c_algorytm_zachlanny.h"

c_algorytm_zachlanny::c_algorytm_zachlanny(std::vector<std::vector<double>> _macierz_odleglosci, bool _wyswietlanie)
{
	macierz_odleglosci = _macierz_odleglosci;
	wyswietlanie = _wyswietlanie;

}

void c_algorytm_zachlanny::znajdz_rozwiazanie(int restauracja, std::vector<int> zamowienia)
{
	int dlugosc_obecnej_trasy = 0;
	

	std::vector<int> obecna_trasa;
	int biezace_miasto = restauracja;
	int poprzednie_miasto = restauracja;

	for (int i = 0; i < macierz_odleglosci.size(); i++) {
		if (biezace_miasto == 999) continue;
		poprzednie_miasto = biezace_miasto;

		obecna_trasa.push_back(biezace_miasto);
		biezace_miasto = ustal_nastepne_miasto(biezace_miasto, obecna_trasa, zamowienia);
		if (biezace_miasto == 999) continue;
		dlugosc_obecnej_trasy = dlugosc_obecnej_trasy + macierz_odleglosci[poprzednie_miasto][biezace_miasto];
	}

	if (wyswietlanie)
	{
		std::cout << "Trasa kuriera:";
		for (size_t i = 0; i < obecna_trasa.size(); i++)
			printf("%d -> ", obecna_trasa[i]);

		std::cout << "\nJej dlugosc to: " << +dlugosc_obecnej_trasy << "\n";
	}
}

int c_algorytm_zachlanny::ustal_nastepne_miasto(int poprzednie_miasto, std::vector<int> trasa, std::vector<int> zamowienia)
{
	int nastepne_miasto = 999;
	int minimalna_odleglosc = 9999999;

	for (unsigned int i = 1; i < macierz_odleglosci.size(); i++) {

		if (!std::count(zamowienia.begin(), zamowienia.end(), i)) continue;

		if (minimalna_odleglosc > macierz_odleglosci[poprzednie_miasto][i]) {
			if (poprzednie_miasto == i) continue;
			if (std::count(trasa.begin(), trasa.end(), i)) continue;
			minimalna_odleglosc = macierz_odleglosci[poprzednie_miasto][i];
			nastepne_miasto = i;

		};

		

	}
	
	return nastepne_miasto;
}