#include<iostream>
#include<string>
#include<fstream>
void print(std::ofstream& toFile, int* M, int l, char ch)
{
	int N[256] = { 0 }, k = -1, i = 0, t = 0;
	int x = l;
	while (x > 0)
	{
		t++;
		x /= 10;
	}

	std::string s;
	for (int i = 0; i < 256; i++)
	{
		if (((ch == 'Y') ? M[i] > 0:M[i] > -1) && i != 10 && i != 13 && i != 9)
		{
			N[++k] = M[i];
			s += char(i);
		}
	}
	int f = k;
	if (M[10] > 0 || ch == 'N') f += 9;
	if (M[9] > 0 || ch == 'N') f += 5;
	if (M[13] > 0 || ch == 'N') f += 15;

	for (i = (f + t - 22); i >= (f + t - 22) / 2; i--)
		toFile << "=";
	toFile << " Symbol Usage Statistics ";
	for (; i >= 0; i--)
		toFile << "=";
	toFile << std::endl;
	for (int i = l; i > 0; --i)
	{
		{
			int g = 0;
			x = i;
			while (x > 0)
			{
				g++;
				x /= 10;
			}

			for (int i = g; i < t; i++)
				toFile << " ";

			toFile << i << "|";
		}

		for (int j = 0; j <= k; ++j)
		{
			if (N[j] >= i) toFile << "*";
			else toFile << " ";
		}
		if (M[10] >= i) toFile << "    *    ";
		else if (ch == 'N' || M[9] > 0 || M[13] > 0)toFile << "         ";
		if (M[9] >= i) toFile << "  *  ";
		else if (ch == 'N' || M[13] > 0)toFile << "     ";
		if (M[13] >= i) toFile << "       *       ";
		else if (ch == 'N')toFile << "               ";
		toFile << "|" << std::endl;
	}
	for (int i = 1; i <= t; i++)
		toFile << " ";
	toFile << "|";
	for (int i = 0; i <= f; i++)
		toFile << "-";
	toFile << "|" << std::endl;
	for (int i = 1; i <= t; i++)
		toFile << " ";
	toFile << "|";
	toFile << s;
	if (M[10] > 0 || ch == 'N') s += "N", toFile << " NewLine ";
	if (M[9] > 0 || ch == 'N') s += "T", toFile << " Tab ";
	if (M[13] > 0 || ch == 'N') s += "R", toFile << " Return-Cursor ";
	toFile << "|" << std::endl << std::endl << "> Number of hidden symbols: " << 256 - s.length() << std::endl;
	return;
}
int main()
{
	std::string file1, file2;
	std::cout << "Input source file name: ";
	std::cin >> file1;
	std::ifstream fromFile(file1);
	if (!fromFile)
	{
		std::cout << "Error! Can not open file " << file1 << std::endl;
		return 1;
	}

	std::cout << "Input destination file name: ";
	std::cin >> file2;
	std::ofstream toFile(file2);
	if (!toFile)
	{
		fromFile.close();
		std::cout << "Error! Can not write to file " << file2 << std::endl;
		return 2;
	}
	int M[256] = { 0 };
	int k;
	while ((k = fromFile.get()) != EOF)
	{
		M[k]++;
	}
	int mx = 0, mn = 0;
	for (int i = 0; i < 256; i++)
	{
		if (M[i] > mx) mx = M[i];
		if (M[i] == 0) mn++;
	}
	char ans;
	if (mn > 0)
	{
		do
		{
			std::cout << "Do you allow us to hide characters that have zero values?(Y-Yes or N-No): ";
			std::cin >> ans;
		} while (ans != 'Y' && ans != 'N');
	}
	print(toFile, M, mx, (mn > 0) ? ans : 'N');

	for (int i = 69 + file2.length(); i > 0; --i)
		std::cout << "=";
	std::cout << std::endl;
	std::cout << "> Statistics have been successfully calculated and printed to file " << file2 << " <" << std::endl;
	for (int i = 69 + file2.length(); i > 0; --i)
		std::cout << "=";

	fromFile.close();
	toFile.close();
	return 0;
}