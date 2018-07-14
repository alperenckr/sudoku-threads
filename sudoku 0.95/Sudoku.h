#ifndef	SUDOKU_H
#define SUDOKU_H

#include <fstream>
#include <vector>
#include <atomic>

namespace sudoku_wt
{
	constexpr int NOBODY_FINISHED = -1;  //Sabit

	struct cell //Sudoku tahtasindaki hucreler
	{
		int value; //Tuttugu deger
		bool changeable; // Tahtada varsayilan olan degerler degistirilemez. 
		std::vector<int> possibleNumbers; //Hucreye yazilabilecek degerler
		cell() {} //Varsayilan yapici fonksiyon
		cell(int value, bool changeable) //Deger yazdigimiz yapici fonksiyon
			: value{ value }, changeable{ changeable }, possibleNumbers() //Yapici fonksiyon ilklendirme listesi
		{
		}
	};

	class Sudoku
	{
		cell board[9][9];	//Sudoku tahtasi
		std::vector < std::pair<int, int>> game_order; //Algoritmanin cozerken izleyecegi yol
		std::vector <std::pair<int, int>> zeros; //En basta sifi olan degerleri tutan vektor

	public:
		static std::atomic_int finished; // Thread durdurmak icin atomik bayrak
		Sudoku(std::fstream &file);      // Dosya alan yapici fonksiyon. Default olan silindi.
		~Sudoku();                       // Yikici fonksiyon
		const std::vector<std::pair<int, int>>& get_game_order() const; //Cozum yolunu alan fonksiyon
		const cell& at(const int & x, const int & y) const;     // Koordinattaki hucreyi donduruyor
		static void solve(Sudoku *sudoku, const int & index);   // Bayrak degistiren ve cozme fonksiyonunu cagiran static fonksiyon
		bool is_finished();

	private:
		bool solve_sudoku(); //Sudoku cozumu
		void init_vectors(); //Hucrelerdeki koyulabilecek degerler vektorunu sirayla doldurur
		void init_game_order(); // Random cozum sirasi ureten fonksiyon
		std::vector<int> set_first_possible_numbers(const int& row, const int& column); //Mumkun olan degerleri hucrelere yazan fonksiyon
		bool find_item(const int & row, const int & column, const int &num); // Hucreye yazilacak degeri bulan fonksiyon
		std::pair<int, int> choose_cell_randomly(); //Random oyun sirasindan degeri sifir olan hucreyi donduren fonksiyon
	};
}
#endif