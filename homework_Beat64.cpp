// base64.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>

char Base64Char(unsigned char b) {
	b &= 0x3F;
	/*	char Base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		return Base64Alphabet[b];
	*/
	if (b < 26) return 'A' + b;
	if (b < 52) return 'a' + (b - 26);
	if (b < 62) return '0' + (b - 52);
	if (b == 62) return '+';
	return '/';
}
char Base64Char2(unsigned char b) {

	if (b >= 'A' && b <= 'Z')return b - 65;
	if (b >= 'a' && b <= 'z')return b - 71;
	if (b >= '0' && b <= '9')return b + 4;
	if (b == '+')return 62;
	if (b == '/')return 63;
	return 0;

}

int Base64Encode(char* inStr, int Len, char* outStr) {
	/*
		inStr - входной строкой
		Len - длина входной строки, количество символов, которые нужно кодировать
		outStr - выходная строка
		Функция возвращает количество символов в выходной строке.
	*/
	int i = 0, j = 0; // i - индекс во входной строке, j - индекс в выходной строке
	unsigned char a1, a2, a3;
	unsigned char b1, b2, b3, b4;

	for (i = 0; i < Len; i += 3) {
		a1 = a2 = a3 = 0;
		b1 = b2 = b3 = b4 = 0;

		a1 = inStr[i];
		if (i + 1 < Len) a2 = inStr[i + 1];
		if (i + 2 < Len) a3 = inStr[i + 2];

		b1 = a1 >> 2;
		b2 = ((a1 & 0x3) << 4) | (a2 >> 4);
		b3 = ((a2 & 0xF) << 2) | (a3 >> 6);
		b4 = a3 & 0x3F;

		outStr[j++] = Base64Char(b1);
		outStr[j++] = Base64Char(b2);
		outStr[j++] = (i + 1 < Len) ? Base64Char(b3) : '=';
		outStr[j++] = (i + 2 < Len) ? Base64Char(b4) : '=';
	}

	return j;
}

int Base64Decode(char* inStr, int Len, char* outStr) {
	int i = 0, j = 0;
	unsigned char a1, a2, a3;
	unsigned char b1, b2, b3, b4;

	for (i = 0; i < Len; i += 4) {
		b1 = Base64Char2(inStr[i]);
		b2 = Base64Char2(inStr[i + 1]);
		b3 = Base64Char2(inStr[i + 2]);
		b4 = Base64Char2(inStr[i + 3]);



		a1 = ((b1 << 2) | b2 >> 4);
		outStr[j++] = a1;

		if (b3 != -1)
		{
			a2 = ((b2 & 0x0F) << 4) | (b3 >> 2);
			outStr[j++] = a2;
		}
		if (b3 != -1)
		{
			a3 = ((b3 << 6) | b4);
			outStr[j++] = a3;
		}

	}
	return j;
}

int main()
{
	/*	char input[] = "Hello world! ";
		char output[256];
		int result;
		result = Base64Encode(input, strlen(input), output);
		output[result] = '\0';
		std::cout << output << std::endl;
	*/
	char inputFileName[1024];
	char outputFileName[1024];
	std::cout << "Input source file name: ";
	std::cin.getline(inputFileName, 1024);

	std::ifstream inFile(inputFileName, std::ios_base::binary);
	if (!inFile) {
		std::cout << "Can not open file " << inputFileName << std::endl;
		return 1;
	}

	std::cout << "Input destination file name: ";
	std::cin.getline(outputFileName, 1024);

	std::ofstream outFile(outputFileName);
	if (!outFile) {
		inFile.close();
		std::cout << "Can not write to file " << outputFileName << std::endl;
		return 2;
	}
	const int block_size = 2;
	char inStr[4 * block_size], outStr[3 * block_size];
	int s;

	inFile.seekg(0, inFile.end);
	int length = inFile.tellg();
	inFile.seekg(0, inFile.beg);
	char* delStr=new char[length];
	inFile.read(delStr, length);
	for (int i = 0; i < length; ++i)
	{
		std::cout << delStr[i] << std::endl;
		if (delStr[i] == '\n' || delStr[i] == '\r')
		{
			for (int j = i; j < length; ++j)
			{
				delStr[j] = delStr[j + 1];
			}
			i--;
		}

	}
	std::cout << delStr;
	std::ofstream (inputFileName).write(delStr, length);
	inFile.close();
	std::ifstream inFile2(inputFileName, std::ios_base::binary);
	while (inFile2.peek() != EOF) {

		inFile2.read(inStr, 4 * block_size); // Попытаемся прочитать из файла 3*block_size байт и записать в inStr
		std::cout << sizeof(inStr);
	
		s = inFile2.gcount(); // Сколько было прочитано при последней попытке
		s = Base64Decode(inStr, s, outStr);
		outFile.write(outStr, s);
		outFile << std::endl;
	}

	inFile2.close();
	outFile.close();
	std::cout << "Encoded - OK" << std::endl;
}
