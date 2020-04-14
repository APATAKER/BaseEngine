#include "sudoku.h"
#include <ctime>
#include <iostream>


template <class T>
T randInRange(T min, T max)
{
    double value =
        min + static_cast <double> (rand())
        / (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
    return static_cast<T>(value);
};


cSudoku::cSudoku(int N, int K)
{
	this->N = N;
	this->K = K;
	double SRNd = sqrt(N);
	this->SRN = static_cast<int>(SRNd);

	for(int i = 0;i<N;i++)
        for(int j=0;j<N;j++)
            map_mat.insert_or_assign(std::pair<int, int>(i, j), 0);

}

void cSudoku::fillValues()
{
	fillDiagonal();

	fillRemaining(0, SRN);

	removeDigits();
}

void cSudoku::fillDiagonal()
{
    srand(time(0));
	for (int i = 0; i < N; i = i + SRN)
		fillBox(i, i);
}

void cSudoku::fillBox(int row, int col)
{
	int num;
	for(int i=0;i<SRN;i++)
	{
		for(int j=0;j<SRN;j++)
		{
			do
			{
				num = /*randomGen(N)%9*/randInRange(1,N+1);
			} while (!unUsedInBox(row, col, num));

			map_mat.insert_or_assign(std::pair<int,int>(row+i,col+j) , num);
		}
	}
}

bool cSudoku::fillRemaining(int i, int j)
{
    if (j >= N && i < N - 1)
    {
        i = i + 1;
        j = 0;
    }
    if (i >= N && j >= N)
        return true;

    if (i < SRN)
    {
        if (j < SRN)
            j = SRN;
    }
    else if (i < N - SRN)
    {
        if (j == (int)(i / SRN) * SRN)
            j = j + SRN;
    }
    else
    {
        if (j == N - SRN)
        {
            i = i + 1;
            j = 0;
            if (i >= N)
                return true;
        }
    }

    for (int num = 1; num <= N; num++)
    {
        if (CheckIfSafe(i, j, num))
        {
            map_mat.insert_or_assign(std::pair<int, int>(i, j), num);
            if (fillRemaining(i, j + 1))
                return true;

            map_mat.insert_or_assign(std::pair<int, int>(i, j), 0);
        }
    }
    return false;
}

void cSudoku::removeDigits()
{
    int count = K;
    while (count != 0)
    {
        int cellId = randInRange(1, (N * N)+1);/*randomGen(N * N)*/;

        int i = (cellId / N);
        int j = cellId % 9;
        if (j != 0)
            j = j - 1;
    	
        if (map_mat.at(std::pair<int, int>(i, j)) != 0)
        {
            count--;
            map_mat.insert_or_assign(std::pair<int, int>(i, j), 0);
           
        }
    }
}

void cSudoku::printSudoku()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << map_mat.at(std::pair<int, int>(i, j)) << " ";
            //System.out.print(mat[i][j] + " ");
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


