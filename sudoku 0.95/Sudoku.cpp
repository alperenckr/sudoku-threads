#include "Sudoku.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace sudoku_wt
{
	

	std::atomic_int Sudoku::finished = NOBODY_FINISHED; //Static flag deger atama

	auto find_big_cell =
		[](const int& row, const int& column) -> std::pair<int, int>
	{
		return { row / 3, column / 3 };
	};   /* Oyun tahtasindaki bir hucrenin hangi 3*3luk hucrede oldugunu bulan lambda ornegi */

	void Sudoku::solve(Sudoku *sudoku, const int & index) //Static cozum cagirma ve flag degistirme fonksiyonu
	{
		sudoku->solve_sudoku();
		finished = index;
	}

	bool Sudoku::is_finished()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (board[i][j].value == 0)
					return false;
			}
		}
		return true;
	}

	std::pair<int, int> Sudoku::choose_cell_randomly() // Sifir olan degeri bulur secer
	{
		for (auto it : game_order) //Vektorde sifir olan ilk degeri dondurur.
		{
			if (board[it.first][it.second].value == 0)
			{
				return { it.first, it.second };
			}
		}
		return { -1,-1 }; //Vektorde sifir -1 dondurur. Sudoku bitmistir
	}

	Sudoku::Sudoku(std::fstream& file)
	{
		file.clear();
		file.seekg(0, std::ios::beg);

		for (auto i = 0; i < 9; i++)
		{
			for (auto j = 0; j < 9; j++)
			{
				if (auto character{ file.get() }; character != '*')
				{
					//Karakter alir ve '*' mi diye bakar.
					auto convert_to_int = [character]() -> int { return (character - '0'); }; // Ascii sayiyi int yapan lambda 
					board[i][j] = { convert_to_int() , false }; // Degitirilemeyecek sayi oyun tahtasinda varsayilan olarak bulunur
				}
				else
				{
					board[i][j] = { 0 , true }; // Verilmeyen sayilar sifirdir. Degistirilebilirdir
					zeros.push_back({ i,j });
				}
			}
			file.get(); // Dosyadaki enterleri okur.
		}
		init_vectors(); //Hucrelerin icindeki vektorlere, hucrelerin alabilecek degerlerini koyar.
		init_game_order();
	}
	bool Sudoku::solve_sudoku()
	{
		if (finished == NOBODY_FINISHED) //Flag degismediyse cozume devam et
		{
			if (auto[row, column] = choose_cell_randomly(); row == -1)//Hiçbir eleman sıfır olmayacak şekilde bakar
			{
				return true; //Cozum bitti
			}
			else
			{
				for (auto iter : board[row][column].possibleNumbers) //Yazilacak hucrenin alabilecegi degerler
				{
					if (find_item(row, column, iter)) // Vektordeki degerler uygun mu
					{
						board[row][column].value = iter; // Uygun yaz
						if (solve_sudoku()) // true donerse
							return true; // Program biter
						board[row][column].value = 0; // Koyulan deger cozume ulastirmadi. Tekrar sifir yap tekrar dene
					}
				}
				return false; // Hala calismadi cozum bulunamadi
			}
		}
		return false;
	}
	bool Sudoku::find_item(const int & row, const int & column, const int & num) //Deger arama
	{
		for (int i = 0; i < 9; i++)
		{
			if (board[i][column].value == num) // satirda o deger var mi
			{
				return false; //varsa uygun degil
			}
			if (board[row][i].value == num) //sutunda o deger var mi
			{
				return false; //varsa uygun degil
			}
		}
		auto[cell_row, cell_column] = find_big_cell(row, column); //Hucrenin bulundugu 3*3luk buyuk hucrenin koordinatini bul
		for (int i = 0; i < 3; i++)  //3*3luk hucreler kontol edilir.
		{
			for (int j = 0; j < 3; j++)
			{

				if (board[cell_row * 3 + i][cell_column * 3 + j].value == num) //Buyuk hucrede var mi
				{
					return false; // varsa uygun degil
				}
			}
		}
		return true;
	}
	Sudoku::~Sudoku() //yikici fonksiyon
	{
	}
	void Sudoku::init_vectors()//Alabilecekleri değerleri vektörlere eklemek için  her bir eleman için fonksiyonu çağırma kısmı
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (board[i][j].value == 0) // Degistirilebilecek degerlere vector atamasi yapilmalidir.
					board[i][j].possibleNumbers = set_first_possible_numbers(i, j); // Degisitirilebilir degerler hucrelere verilir
			}
		}
	}
	void Sudoku::init_game_order() // Random oyun sirasi bulma
	{
		
		while (!zeros.empty()) // Zeros vektorundeki tum degerler eklenmeli
		{		
			std::random_device rd; // Deterministik olmayan random uretici
			std::uniform_int_distribution<int> dist(0, static_cast<int>(zeros.size()-1)); //uniform dagilimla sayi uretir
			int it = dist(rd); //random generatorla uniform degilimdan sayi secilir
			game_order.push_back(std::move(zeros[it])); //Secilen random sayidaki deger game ordera yazilir
			zeros.erase(zeros.begin() + it); //Bu deger zeros vektorunden silinir.
		} //Zeros vektorunde hic deger kalmayana kadar devam edecek
	}
	const std::vector<std::pair<int, int>>& Sudoku::get_game_order() const  //Oyunun oynanma sirasini donduren fonksiyon
	{
		return game_order;
	}
	const cell& Sudoku::at(const int & x, const int & y) const //Bulunan koordinattaki hucreyi donduren fonksiyon
	{
		return board[x][y];
	}

	std::vector<int> Sudoku::set_first_possible_numbers(const int & row, const int & column)
	{
		std::vector<int> possible_numbers{ 1,2,3,4,5,6,7,8,9 }; // Alinebilecek degerlerin ilk hali
		for (int i = 0; i < 9; ++i) // Satirda onceden koyulmus degerler bulunur ve vectorden silinir
		{
			auto iter{ std::find(possible_numbers.begin(), possible_numbers.end(), board[row][i].value) };
			/*std::find konteynirlarda arama yapar*/
			/*vectorde o deger var mi diye bakar */
			/*iterator dondurur*/
			if (iter != possible_numbers.end()) /*Eger std::find deger bulamazsa end() iteratoru dondurur.*/
			{
				possible_numbers.erase(iter); //std::vector::erase algoritmasi 
			}
		}
		for (int i = 0; i < 9; ++i) // Sutunda onceden koyulmus degerler bulunur ve vectorden silinir.
		{
			auto iter{ std::find(possible_numbers.begin(), possible_numbers.end(), board[i][column].value) };
			/*std::find konteynirlarda arama yapar*/
			/*vectorde o deger var mi diye bakar */
			/*iterator dondurur*/
			if (iter != possible_numbers.end()) /*Eger std::find deger bulamazsa end() iteratoru dondurur.*/
			{
				possible_numbers.erase(iter); //std::vector::erase algoritmasi 
			}
		}

		auto[cell_row, cell_column] = find_big_cell(row, column);	// structured binding
																	/*
																	find_big_cell lambdas? std::pair<int,int> dondurur.
																	C++17 structured bindings ozelligi ile pairi iki parcaya boler ve degerlere atar.
																	*/
		for (int i = 0; i < 3; i++)  //3*3luk hucreler kontol edilir.
		{
			for (int j = 0; j < 3; j++)
			{
				auto iter = std::find(possible_numbers.begin(), possible_numbers.end(),
					board[cell_row * 3 + i][cell_column * 3 + j].value); //3*3luk buyuk hucrenin icindeki hucrelerdeki
																		 // degerler aranir
				if (iter != possible_numbers.end()) // eger deger bulunursa
				{
					possible_numbers.erase(iter); // deger silinir
				}
			}
		}
		return possible_numbers; //Vectorun son hali dondurulur
	}
}

