// Вариант 1: класс комплексное число
#include <iostream>
#include <cmath>

class Complex{
private:
	long double m_a;
	long double m_b;
public:
	Complex() : m_a(0), m_b(0) { }
	
	Complex(double a, double b = 0)
		: m_a(a), m_b(b) { }

	friend std::ostream& operator<< (std::ostream &out, const Complex &num);
	friend std::istream& operator>> (std::istream &in, Complex &num);
	Complex& operator +=(const Complex &num);
	Complex& operator -=(const Complex &num);
	Complex conj();
	friend const Complex operator+(const Complex &left, const Complex &right);
	friend const Complex operator-(const Complex &left, const Complex &right);
	friend const Complex operator*(const Complex &left, const Complex &right);
	friend const Complex operator/(const Complex &left, const Complex &right);
	friend bool operator >(const Complex &left, const Complex &right);
	friend bool operator <(const Complex &left, const Complex &right);
	friend bool operator ==(const Complex &left, const Complex &right);
	friend bool operator !=(const Complex &left, const Complex &right);
	friend bool operator >=(const Complex &left, const Complex &right);
	friend bool operator <=(const Complex &left, const Complex &right);
	
	friend Complex operator"" _d(long double val);
    friend Complex operator"" _i(long double val);
};

// Операция +=
Complex& Complex::operator +=(const Complex &num)
{
	m_a += num.m_a;
	m_b += num.m_b;
	return *this;
}

// Операция -=
Complex& Complex::operator -=(const Complex &num)
{	
	m_a -= num.m_a;
	m_b -= num.m_b;
	return *this;
}

//Сложение
const Complex operator+(const Complex &left, const Complex &right)
{
	return Complex(left.m_a + right.m_a, left.m_b + right.m_b);
}

// Вычитание
const Complex operator-(const Complex &left, const Complex &right)
{
	return Complex(left.m_a - right.m_a, left.m_b - right.m_b);
}

//Умножение
const Complex operator*(const Complex &left, const Complex &right)
{
	return Complex(left.m_a * right.m_a - left.m_b * right.m_b, left.m_a * right.m_b + left.m_b * right.m_a);
}

//Деление
const Complex operator/(const Complex &left, const Complex &right) 
{
	return Complex((left.m_a * right.m_a + left.m_b * right.m_b) / (right.m_a * right.m_a + right.m_b * right.m_b), (right.m_a * left.m_b - left.m_a * right.m_b) / (right.m_a * right.m_a + right.m_b * right.m_b));
}

// Вывод
std::ostream& operator<< (std::ostream &out, const Complex &num)
{
	if(num.m_b > 0)
		out << num.m_a << "+" << num.m_b << "i";
	else if(num.m_b < 0)
		out << num.m_a << "" << num.m_b << "i";
	else
		out << num.m_a;
	return out; 
}

// Ввод
std::istream& operator>> (std::istream &in, Complex &num)
{
	in >> num.m_a;
	in >> num.m_b;
	return in;
}

// Сопряженное: (a, b) = (a, -b)
Complex Complex::conj()
{
	return Complex(m_a, -m_b);
}
/* 
Операция >: сравниваются тип double, поэтому идет проверка по модулю разности
Модуль комплексного числа module = sqrt(a^2 + b^2)  
*/
bool operator>(const Complex &left, const Complex &right)
{
	double r1 = sqrt(left.m_a * left.m_a + left.m_b * left.m_b);
	double r2 = sqrt(right.m_a * right.m_a + right.m_b * right.m_b);
	if(fabs(r1 - r2) < 0.000001) return false;
	return (r1 > r2);
}

// Операция <
bool operator<(const Complex &left, const Complex &right)
{
	double r1 = sqrt(left.m_a * left.m_a + left.m_b * left.m_b);
	double r2 = sqrt(right.m_a * right.m_a + right.m_b * right.m_b);
	if(fabs(r1 - r2) < 0.000001) return false;
	return (r1 < r2);

}

// Операция ==: числа равны, если a1 == a2 и b1 == b2 (сравнение также осуществляется по модулю)
bool operator==(const Complex &left, const Complex &right)
{
	return ((fabs(left.m_a - right.m_a) < 0.000001) && (fabs(left.m_b - right.m_b) < 0.000001));
}

// Операция !=
bool operator!=(const Complex &left, const Complex &right)
{
	return ((fabs(left.m_a - right.m_a) > 0.000001) || (fabs(left.m_b - right.m_b) > 0.000001));
}

// Операция >= - сравнение осуществляется по модулю
bool operator>=(const Complex &left, const Complex &right)
{
	double r1 = sqrt(left.m_a * left.m_a + left.m_b * left.m_b);
	double r2 = sqrt(right.m_a * right.m_a + right.m_b * right.m_b);
	if(fabs(r1 - r2) < 0.000001) return true;
	return (r1 > r2);
}

// Операция <= - сравнение осуществляется по модулю
bool operator<=(const Complex &left, const Complex &right) 
{
	double r1 = sqrt(left.m_a * left.m_a + left.m_b * left.m_b);
	double r2 = sqrt(right.m_a * right.m_a + right.m_b * right.m_b);
	if(fabs(r1 - r2) < 0.000001) return true;
	return (r1 < r2);

}

// Пользовательский литерал действительной части
Complex operator"" _d(long double val)
{
    return Complex(static_cast<double>(val), 0);
}

// Пользовательский литерал мнимой части
Complex operator"" _i(long double val)
{
	return Complex(0, static_cast<double>(val));
}

// Меню
void print_menu()
{
	std::cout << "\nMENU\n";
	std::cout << "1. Print complex number\n"
		 << "2. Add 2 numbers\n"
		 << "3. Subtract 2 numbers\n"
		 << "4. Multiply 2 numbers\n5. Divide 2 numbers\n"
		 << "6. Compare 2 modules of numbers\n7. Find conjugate number\n"
		 << "8. Check numbers for equality\n"
		 << "9. Check numbers for inequality\n"
		 << "10. Print menu\n0. Exit\n" << std::endl;
}

// Проверка корректности ввода комплексного числа
Complex inputValue()
{
	while(true){
		std::cout << "Input complex number  ( , ):  ";
		Complex a;
		std::cin >> a;
		if (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "Values are uncorrect. You can input only numbers. Try again.\n";
        }
        else{
            std::cin.ignore(32767,'\n'); 
            return a;
        }
	}
}

// Проверка ввода целого числа
int input_Value()
{
	while(true){
		std::cout << "Input integer value:  ";
		int a;
		std::cin >> a;
		if (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "Value is uncorrect. You can input only integer number. Try again.\n";
        }
        else{
            std::cin.ignore(32767,'\n'); 
            return a;
        }
	}
}

int main()
{
	// Пример пользовательского литерала
	Complex x = 5.0_d + 10.0_i;
	std::cout << "\nUser literal example: " << x << '\n' << std::endl;

	Complex num, num2;
	num = inputValue();
	
	int k = 1;
	print_menu();
	while(k){
		std::cout << "\nInput variant: ";
		k = input_Value();
		switch(k){
			case 0: break;
			case 1: std::cout << "\nComplex number: " << num << std::endl;
			case 2: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				std::cout << '(' << num  << ')' << " + " << '(' << num2  << ") = ";
				num = num + num2;
				std::cout << num << std::endl;
				break;
			}
		 	case 3: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				std::cout << '(' << num  << ')' << " - " << '(' << num2  << ") = ";
				num = num - num2;
				std::cout << num << std::endl;
				break;
			}
		 	case 4: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				std::cout << '(' << num  << ')' << " * " << '(' << num2  << ") = ";
				num = num * num2;
				std::cout << num << std::endl;
				break; }
			case 5: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				std::cout << '(' << num  << ')' << " / " << '(' << num2  << ") = ";
				num = num / num2;
				std::cout << num << std::endl;
				break; }
			case 6: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				if(num > num2) std::cout << "Module: (" << num  << ')' << " > " << '(' << num2 << ')' << std::endl;
				else if(num < num2) std::cout << "Module: (" << num  << ')' << " < " << '(' << num2 << ')' << std::endl;
				else std::cout << "Module: (" << num  << ')' << " = " << '(' << num2 << ')' << std::endl;
				break; }
			case 7: {
				std::cout << "\nConjugate number for " << num << " is ";
				num = num.conj();
				std::cout << num << std::endl;
				break; }
			case 8: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				if(num == num2) std::cout << num << " Equal " << num2 << std::endl;
				else std::cout << num << " Not equal " << num2 << std::endl;
				break; }
			case 9: {
				std::cout << "\nInput second number: ";
				num2 = inputValue();
				if(num != num2) std::cout << num << " Not equal " << num2 << std::endl;
				else std::cout << num << " Equal " << num2 << std::endl;
				break; }
			case 10: print_menu(); break;
			default:
				std::cout << "\nInput 0 - 10 (10. Print menu)" << std::endl; break;
		}
	}
	return 0;
}