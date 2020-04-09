#pragma once

#include <vector>
#include <map>

class cSudoku
{
	int N;
	int SRN;
	int K;
	std::map<std::pair<int, int>,int> map_mat;
public:
	cSudoku(int N, int K);

	void fillValues();

	void fillDiagonal();

	void fillBox(int row, int col);

	bool fillRemaining(int i, int j);

	void removeDigits();

	void printSudoku();









	bool unUsedInBox(int rowStart,int colStart,int num)
	{
		for (int i = 0; i < SRN; i++)
			for (int j = 0; j < SRN; j++)
				if (map_mat.at(std::pair<int, int>(rowStart + i, colStart + j)) == num)
					return false;
		return true;
	}
	int randomGen(int num)
	{
		return static_cast<int>(floor(rand() * (num + 1)));
	}

	bool CheckIfSafe(int i, int j, int num)
	{
		return (unUsedInRow(i, num) &&
			unUsedInCol(j, num) &&
			unUsedInBox(i - i % SRN, j - j % SRN, num));
	}
	bool unUsedInRow(int i, int num)
	{
		for (int j = 0; j < N; j++)
			if (map_mat.at(std::pair<int, int>(i,j)) == num)
				return false;
		return true;
	}
	bool unUsedInCol(int j, int num)
	{
		for (int i = 0; i < N; i++)
			if (map_mat.at(std::pair<int, int>(i, j)) == num)
				return false;
		return true;
	}
	

};
