/*
Иванова, М8О-201Б
7 вариант:
Создать класс BitString для работы с 128-битовыми строками. Битовая строка должна быть представлена двумя полями типа unsigned long long. 
Реализовать операции для работы с битами: and, or, xor, not - сдвиг влево shiftLeft, сдвиг вправо shiftRight на заданное количество битов. 
Реализовать операцию вычисления количества единичных битов, операции сравнения по количеству единичных битов. 
Реализовать операцию проверки включения.
*/
#include <iostream>
#include <cmath>
#include <string>

typedef unsigned long long u64;

class BitString{
private:
	u64 number1;
	u64 number2;

public:
	BitString() : number1(0), number2(0) { }
	
	BitString(std::string &str)
	{ 
		int n = str.length(); 
		if(n <= 64){ // number1 = 0
			int x = 0; number2 = 0;
			for(int i = n - 1; i >= 0; --i){
				if(str[i] == '1') number2 += (1 *static_cast<u64>(pow(2,x)));
				x++;
			}
			number1 = 0;
		}
		else{ // number1 и number2 ненулевые
			number2 = 0; number1 = 0;
			int x = 0, i = 0;
			while(i < 64) {
				if(str[n - i - 1] == '0') number2 += 0;
				else number2 += (1 * static_cast<unsigned long long>(pow(2, i)));
				i++;
			}
			for(i = n - 65; i >= 0; i--){
				if(str[i] == '0') number1 += 0;
				else number1 += (1*static_cast<unsigned long long>(pow(2,x)));
				x++;
			}
		}
	}

	u64 getNum1() { return number1; } // возвращает number1
	
	u64 getNum2() { return number2; } // возвращает number2

	void andBtStr(std::string &str)  // операция "и"
	{
		BitString bitstring(str);
		number1 &= bitstring.getNum1();
		number2 &= bitstring.getNum2();
	}

	void orBtStr(std::string &str) // операция "или"
	{
		BitString bitstring(str);
		number1 |= bitstring.getNum1(); 
		number2 |= bitstring.getNum2();
	}

	void xorBtStr(std::string &str) // операция "исключающее или"
	{
		BitString bitstring(str);
		number1 ^= bitstring.getNum1(); 
		number2 ^= bitstring.getNum2();
	}

	void notBtStr() // операция "нет"
	{
		number1 = ~number1;
		number2 = ~number2;
	}

	int bit(int i) // получение i бита строки
	{ 
		return i < 64 ? ((number2 >> i) & 1) : (((number1 >> (i - 64)) & 1));
	}

	void shiftleft(int x) // сдвиг влево
	{
		while(x > 0){
			int k = (number2 >> 63) & 1;
			number1 = (number1 << 1) | k;
			number2 = number2 << 1;
			x--;
		}
	}

	void shiftright(int x) // сдвиг вправо
	{
		unsigned long long i = 1;
		while(x > 0){
			int k_ = number1 & 1;
			number1 >>= 1;
			number2 = (number2 >> 1) | (i << (64 - k_));
			x--;
		}
	}

	void printBtStr() //печать строки
	{
		std::cout << "Bitstring: ";
		std::string rez = "";
		if(number1 == 0 && number2 == 0) std::cout << 0 << std::endl;
		else if(number1 == 0){
			int i = 63;
			while(true){
				if(((number2 >> i) & 1) == 1) break;
				i--;
			}
			for(int j = i; j >= 0; j--)
				std::cout << ((number2 >> j) & 1);
			std::cout << std::endl;
		}
		else{
			int i = 63;
			while(true){
				if(((number1 >> (i - 64)) & 1) == 1) break;
				i--;
			}
			for(int j = i; j >= 0; --j)
				std::cout << ((number1 >> (j - 64)) & 1);
			for(int j = 63; j >= 0; --j)
				std::cout << ((number2 >> j) & 1);
			std::cout << std::endl;
		}
	}

};

void print_menu()
{
	std::cout << "\nMENU\n";
	std::cout << "1. Print bitstring\n"
		 << "2. Operator AND\n"
		 << "3. Operation OR\n"
		 << "4. Operation XOR\n5. Operation NOT\n"
		 << "6. Operation i bit\n7. Operation shiftleft\n"
		 << "8. Operation shiftright\n9. Print MENU\n0. Exit\n" << std::endl;
}

int main()
{
	std::string str;
	u64 a, b;
	int c;
	
	std::cout << "Input bonstring: ";
	std::getline(std::cin, str);
	BitString bstr(str);
	
	print_menu();
	int k = 1;
	while(k){
		std::cout << "Input variant: ";
		std::cin >> k;
		switch(k){
		case 1:{
			bstr.printBtStr();
			break;
		}
		case 2:{
			std::cout << "Input second bitstring for and: " << std::endl;
			std::cin >> str;
			bstr.andBtStr(str);
			break;
		}
		case 3:{
			std::cout << "Input second bitstring for or: " << std::endl;
			std::cin >> str;
			bstr.orBtStr(str);
			break;
		}
		case 4:{
			std::cout << "Input second bitstring for xor: " << std::endl;
			std::cin >> str;
			bstr.xorBtStr(str);
			break;
		}
		case 5:{
			bstr.notBtStr();
			break;
		}
		case 6:{
			std::cout << "Input number of bit: ";
			int a1;
			std::cin >> a1;	
			std::cout << a1 << " bit = " << bstr.bit(a1) << std::endl;
			break;
		}
		case 7:{
			std::cout << "Input value for leftshift: ";
			int a1;
			std::cin >> a1;
			bstr.shiftleft(a1);
			break;
		}
		case 8:{
			std::cout << "Input value for rightshift: ";
			int a1;
			std::cin >> a1;
		 	bstr.shiftright(a1);
			break;
		}
		case 9:{
			print_menu();
			break;
		}
		default: break;
	}
}
	return 0;
}