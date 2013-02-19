#include <stdio.h>

#define MAX_NUMBER 9

int array[MAX_NUMBER][MAX_NUMBER] = {
	{8, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 3, 6, 0, 0, 0, 0, 0},
	{0, 7, 0, 0, 9, 0, 2, 0, 0},
	{0, 5, 0, 0, 0, 7, 0, 0, 0},
	{0, 0, 0, 0, 4, 5, 7, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 3, 0},
	{0, 0, 1, 0, 0, 0, 0, 6, 8},
	{0, 0, 8, 5, 0, 0, 0, 1, 0},
	{0, 9, 0, 0, 0, 0, 4, 0, 0},
};

void solve(int row, int col)
{
	// find zero number
	while (1)
	{
		if (col >= MAX_NUMBER)
		{
			++row;
			col = 0;
		}

		if (row >= MAX_NUMBER)
		{
			for (int i = 0; i < MAX_NUMBER; ++i)
			{
				for (int j = 0; j < MAX_NUMBER; ++j)
				{
					printf("%d ", array[i][j]);
				}
				printf("\n");
			}
			printf("OK\n");
			return;
		}

		if (array[row][col] == 0)
		{
			break;
		}

		++col;
	}

	while (array[row][col] < MAX_NUMBER)
	{
		++array[row][col];

		// the number is valid
		bool flag = true;

		// row
		for (int i = 0; i < MAX_NUMBER; ++i)
		{
			if (i != col && array[row][i] == array[row][col])
			{
				flag = false;
				break;
			}
		}

		// col
		for (int i = 0; i < MAX_NUMBER; ++i)
		{
			if (i != row && array[i][col] == array[row][col])
			{
				flag = false;
				break;
			}
		}

		// nine cells
		int diff_row = row % 3;
		int diff_col = col % 3;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (diff_row != i && diff_col != j && array[row-diff_row+i][col-diff_col+j] == array[row][col])
				{
					flag = false;
					break;
				}
			}
		}

		if (!flag)
		{
			continue;
		}

		solve(row, col+1);
	}

	array[row][col] = 0;
}

int main()
{
	solve(0, 0);
	return 0;
}
