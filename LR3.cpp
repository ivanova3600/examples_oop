#include <iostream>
#include <cmath>
#include <vector>

struct Point{ // класс для описания координаты точки
	double m_x, m_y;
	Point(double x = 0, double y = 0) : m_x(x), m_y(y) { }
	//вывод точки
	friend std::ostream& operator<<(std::ostream &out, const Point &p)
	{
		out << '(' << p.m_x << ',' << p.m_y << ')';
		return out;
	}
	//ввод с проверкой
	friend std::istream& operator>>(std::istream &in, Point &p)
	{
		while(true){
			in >> p.m_x;
			if (in.fail()){
            	in.clear(); in.ignore(32767,'\n');
            	std::cout << "Value is uncorrect. You can input only numbers. Try again.\n";
        	}
        	else break;
        }
		while(true){
			in >> p.m_y;
			if (in.fail()){ in.clear(); in.ignore(32767,'\n');
            	std::cout << "Value is uncorrect. You can input only numbers. Try again.\n";
        	}
        	else break;
        }
		return in;
	}
};
//проверка ввода для int
int inputValue()
{
	while(true){
		int a;
		std::cin >> a;
		if (std::cin.fail()){ std::cin.clear(); std::cin.ignore(32767,'\n');
            std::cout << "Value is uncorrect. You can input only numbers. Try again.\n";
        }
        else{
            std::cin.ignore(32767,'\n'); 
            return a;
        }
	}
}
//проверка на выпуклость в зависимости от обхода
bool checkV(Point vert[], int n)
{
	int p;
	double a = (vert[0].m_x-vert[n-1].m_x)*(vert[1].m_y-vert[0].m_y)-(vert[0].m_y-vert[n-1].m_y)*(vert[1].m_x-vert[0].m_x);
	if( a > 0) p = 1;
	else p = 0;
	a = (vert[n-1].m_x-vert[n-2].m_x)*(vert[0].m_y-vert[n-1].m_y)-(vert[0].m_y-vert[n-1].m_y)*(vert[n-1].m_x-vert[n-2].m_x);
	if( a > 0 && p == 0) return false;
	if(a < 0 && p == 1) return false;
	for(int i = 1; i < n - 1; ++i){
		a = (vert[i].m_x-vert[i-1].m_x)*(vert[i+1].m_y-vert[i].m_y)-(vert[i].m_y-vert[i-1].m_y)*(vert[i+1].m_x-vert[i].m_x);
		if(a > 0 && p == 0) return false;
		if(a < 0 && p == 1) return false;
	}
	return true;
}
//базовый класс
class Figure{
protected: 
	Point *vert; //массив вершин
	int m_type; //тип
public: 
	Figure(int type = 0) : m_type(type) { 
	  vert = new Point[m_type];
	}
	virtual Point centregeom() = 0; //геометрический центр
	// вывод вершин
	friend std::ostream& operator<< (std::ostream &out, Figure &f) { 
		for(int i = 0; i < f.m_type; ++i) out << f.vert[i] << ' ';
		return out; 
	}
	// ввод вершин
	friend std::istream& operator>> (std::istream &in, Figure &f) { 
		while(true){
			for(int i = 0; i < f.m_type; ++i) in >> f.vert[i];
				if(checkV(f.vert, f.m_type) == true) break;
			else std::cout << "Figure is not true" << std::endl;
		}		
		return in; 
	}
	virtual double calcArea() = 0; //площадь
	virtual ~Figure() { delete[] vert;  /* std::cout << "delete" << std::endl; */}
	int getType() { return m_type; } // возвращает тип
};
//класс пятиугольник
class Pentagon : public Figure{
public:
	Pentagon() : Figure(5) { }
	Pentagon(Point vert1[]) : Figure(5) {
		for(int i = 0; i < m_type; ++i)  vert[i] = vert1[i];		 
	}	
	Pentagon(const Pentagon &p) : Figure(5) {
		for(int i = 0; i < m_type; ++i)  vert[i] = p.vert[i];
	}
	Point centregeom() override{ // геометрический центр
		return Point((vert[0].m_x + vert[1].m_x + vert[2].m_x + vert[3].m_x + vert[4].m_x)/5, 
			(vert[0].m_y + vert[1].m_y + vert[2].m_y + vert[3].m_y + vert[4].m_y)/5);
	}
	double calcArea() override{ // площадь
		double s = 0;
		for(int i = 0; i < 4; ++i)  s += (vert[i].m_x + vert[i+1].m_x) * (vert[i].m_y - vert[i+1].m_y);
		s += (vert[4].m_x + vert[0].m_x) * (vert[4].m_y - vert[0].m_y);
		return 0.5 * (fabs(s));
	}
	~Pentagon() {  }
};
// класс шестиугольник
class Hexagon : public Figure{
public:
	Hexagon(): Figure(6) { 
	}
	Hexagon(Point vert1[]) : Figure(6) {
		for(int i = 0; i < m_type; ++i)  vert[i] = vert1[i];		 
	}		
	Hexagon(const Hexagon &h) : Figure(6) {
		for(int i = 0; i < m_type; ++i)  vert[i] = h.vert[i];
	}
	Point centregeom() override{ // геометрический центр
		return Point((vert[0].m_x + vert[1].m_x + vert[2].m_x + vert[3].m_x + vert[4].m_x + vert[5].m_x)/6,
		 (vert[0].m_y + vert[1].m_y + vert[2].m_y + vert[3].m_y + vert[4].m_y + vert[5].m_y)/6);
	}
	double calcArea() override{ // площадь
		double s = 0;
		for(int i = 0; i < 5; ++i) s += (vert[i].m_x + vert[i+1].m_x) * (vert[i].m_y - vert[i+1].m_y);
		s += (vert[5].m_x + vert[0].m_x) * (vert[5].m_y - vert[0].m_y);
		return 0.5 * (fabs(s));
	}
	~Hexagon() { }
};
// класс восьмиугольник
class Octagon : public Figure{
public:
	Octagon() : Figure(8) { }
	Octagon(Point vert1[]) : Figure(8) {
		for(int i = 0; i < m_type; ++i) vert[i] = vert1[i];
	}	
	Octagon(const Octagon &o) : Figure(8) {
		for(int i = 0; i < m_type; ++i) vert[i] = o.vert[i];
	}
	Point centregeom() override{ // геометрический центр
		return Point((vert[0].m_x + vert[1].m_x + vert[2].m_x + vert[3].m_x + vert[4].m_x + vert[5].m_x + vert[6].m_x + vert[7].m_x)/8,
		 (vert[0].m_y + vert[1].m_y + vert[2].m_y + vert[3].m_y + vert[4].m_y + vert[5].m_y + vert[6].m_y + vert[7].m_y)/8);
	}

	double calcArea() override{ // площадь
		double s = 0;
		for(int i = 0; i < 7; ++i)  s += (vert[i].m_x + vert[i+1].m_x) * (vert[i].m_y - vert[i+1].m_y);	
		s += (vert[7].m_x + vert[0].m_x) * (vert[7].m_y - vert[0].m_y);
		return 0.5 * (fabs(s));
	}
	~Octagon() { }
};
// меню
void print_menu()
{
	std::cout << "MENU\n";
	std::cout << "1. Insert figure\n"
		 << "2. Delete figure\n"
		 << "3. Print array\n"
		 << "4. Calculate geometr center of figure\n5. Calculate area of figure\n"
		 << "6. Calculate all areas of figures\n"
		 << "7. Print menu\n0. Exit\n" << std::endl;
}

int main()
{
	int type;
	std::vector<Figure*> figure; // коллеция фигур

	int j = 7; print_menu();

	while(j){
		std::cout << "Input variant: ";
		j = inputValue();
		switch(j){
			case 1:{ // вставка элемента в коллекцию
				std::cout << "Input type, then input coordinates (x1 y1 x2 y2 ...): ";
				type = inputValue();
				switch(type){
					case 5:{ // пятиугольник
						Pentagon *p = new Pentagon(); std::cin >> *p;
						figure.push_back(new Pentagon(*p)); delete p;
						break;
					}
					case 6:{ //шестиугольник
						Hexagon *p = new Hexagon(); std::cin >> *p;
						figure.push_back(new Hexagon(*p)); delete p;
					 	break;
					}
					case 8:{ //восьмиугольник
						Octagon *p = new Octagon(); std::cin >> *p;
						figure.push_back(new Octagon(*p)); delete p;
					 	break;
					} 
					default: break;
				}
				break;
			}
			case 2:{ //удаление по индексу
				std::cout << "Input index of deleted figure: ";
				size_t ind; ind = inputValue();
				if(ind < figure.size()) {
					delete figure[ind];
					figure.erase(figure.begin() + ind);
				}	
				break;
			}
			case 3:{ //печать коллекции
				std::cout << "Array:\n";
				for(size_t i = 0; i < figure.size(); ++i) std::cout << figure[i]->getType() << "-agon: " << *(figure[i]) << std::endl;
				std::cout << std::endl;
				break;
			}
			case 4:{ //геометрический центр
				std::cout << "Input index of geomcenter figure: ";
				size_t ind; ind = inputValue();
				if(ind < figure.size())
					std::cout << "Geomcenter: " << figure[ind]->centregeom() << std::endl;
				break;
			}
			case 5:{ //площадь одной фигуры
				std::cout << "Input index of area figure: ";
				size_t ind; ind = inputValue();
				if(ind < figure.size())
					std::cout << "Area: " << figure[ind]->calcArea() << std::endl;
				break;
			} 
			case 6:{ //общая площадь
				double s = 0;
				for(size_t i = 0; i < figure.size(); ++i){
					s += figure[i]->calcArea();
				}
				std::cout << "All area: " << s << std::endl;
				break;
			}
			case 7: print_menu(); break;
			default: break;
		}
	}
	
	for(size_t i = 0; i < figure.size(); ++i) delete figure[i];
	
	return 0;
}