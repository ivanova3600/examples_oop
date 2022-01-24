// 16 factory.cpp memento.cpp
#include <iostream>
#include <utility>
#include <memory>
#include <cmath>
#include <string>
#include <fstream>
#include <stack>
#include <vector>

template <class T>
class Figure{
public:
	virtual double square() = 0;
	virtual void print(std::ostream &out) = 0;
	virtual void read(std::istream &in) = 0;
};

template <class T>
class Triangle : public Figure<T> {
private:
	T a;
public:
	Triangle(T a_) : a(a_) { }
	Triangle(const Triangle<T> &tr){
		a = tr.a;
	}
	void print(std::ostream &out) override {
		out << "0 " << a << std::endl;
	}	
	double square() override { 
		return sqrt(3)/4*static_cast<double>(a*a); 
	}
	void read(std::istream &in) override {
		in >> a;
	}
};

template <class T>
class Square : public Figure <T> {
private:
	T a;
public:
 	Square(T a_) : a(a_) { }
 	
 	void print(std::ostream &out) override {
		out << "1 " << a << std::endl;
	}
	
	double square() override{
		return static_cast<double>(a*a);
	}
	void read(std::istream &in) override {
		in >> a;
	}
};

template <class T>
class Octagon : public Figure<T> {
private:
	T a;
public:
	Octagon(T a_) : a(a_) { }
	
	void print(std::ostream &out) override {
		out << "2 " << a << std::endl;
	}
	
	double square() override{
		return 2 * static_cast<double>(a*a)*(1+sqrt(2));
	}
	void read(std::istream &in) override {
		in >> a;
	}
};

enum class FigureType{
	Triangle = 0,
	Square = 1,
	Octagon = 2
};

template <class T>
struct Factory{
	static std::shared_ptr<Figure<T>> create(FigureType t, T value){
		switch(t){
			case FigureType::Triangle:
				return std::make_shared<Triangle<T>>(value);
			case FigureType::Square:
				return std::make_shared<Square<T>>(value);
			case FigureType::Octagon:
				return std::make_shared<Octagon<T>>(value);
		}
		return nullptr;
	}
};

template <class T>
struct Memento
{
	std::vector<std::shared_ptr<Figure<T>>> state;
	Memento() { }
	Memento(const std::vector<std::shared_ptr<Figure<T>>> &other) : state(other) { }
};

template <class T>
struct Originator
{
	std::stack<Memento<T>> memento;
	void createMemento(const std::vector<std::shared_ptr<Figure<T>>> &state){
		memento.emplace(state);
	}
	std::vector<std::shared_ptr<Figure<T>>> restore(){
		if(!memento.empty()){
			std::vector<std::shared_ptr<Figure<T>>> result = memento.top().state;
			memento.pop();
			return result;
		}
		else throw std::logic_error("undo stack empty");
	}
};

void print_menu()
{
	std::cout << "MENU\n";
	std::cout << "1. Create new document\n"
		 << "2. Import\n"
		 << "3. Export\n"
		 << "4. Create graphics\n"
		 << "5. Delete graphics\n"
		 << "6. Print\n"
		 << "7. Print menu\n"
		 << "8. Undo\n"
		 << "0. Exit\n" << std::endl;
}

int main()
{
	Originator<int> origin;
	std::vector<std::shared_ptr<Figure<int>>> g;
	std::string name;
	std::ofstream file;
	std::ifstream i_file;
	int type = 0, a = 5;

	int value;
	
	print_menu();
	while(true){
		std::cout << "Input variant: ";
		std::cin >> value;
		if(!value) break;
		switch (value){
			case 1: {
				std::cout << "Input filename: ";
				std::cin >> name;
				i_file.open(name);
				if(!i_file.is_open()){
					file.open(name); 
					if(file.is_open()) std::cout << "file '" << name << "' creates" << std::endl;
					else std::cout << "file '" << name << "' can't create" << std::endl;
					file.close();
				}
				else std::cout << "file '" << name << "' exists" << std::endl;
				i_file.close();
				break;
			}
			case 2: {
				std::cout << "Input import filename: ";
				std::cin >> name;
				i_file.open(name);
				if(i_file.is_open()){
					while(!i_file.eof()){
						i_file >> type >> a;
						std::shared_ptr<Figure<int>> figure = Factory<int>::create((FigureType)type, (int)a);
						origin.createMemento(g);
						g.push_back(figure);
					}
				}
				else std::cout << "Can't open file" << std::endl;
				i_file.close();
				break; 
			}
			case 3: {
				std::cout << "Input import filename: ";
				std::cin >> name;
				file.open(name, std::ios::app);
				if(file.is_open())
					for(auto &elem : g) 
						elem->print(file);
				else std::cout << "Can't open file" << std::endl;
				file.close();
				break;
			}  
			case 4: {
				std::cout << "Input type of figure ( 0 - Triangle, 1 - Square, 2 - Octagon) and length of side: ";
				std::cin >> type >> a;
				std::shared_ptr<Figure<int>> figure = Factory<int>::create((FigureType)type, (int)a);
				origin.createMemento(g);
				g.push_back(figure);
				std::cout << "Create figure" << std::endl;
				break;
			}
			case 5: {
				std::cout << "Input index of figure: ";
				std::cin >> a;
				if(a >= 0 && a < g.size()){
					origin.createMemento(g);
					g.erase(g.begin() + a);
					std::cout << "Delete figure" << std::endl;
				}
				else std::cout << "Uncorrect index" << std::endl;
				break;
			}
			case 6: {
				for(auto &elem : g)
					elem->print(std::cout);
				break;
			}
			case 7: {
				print_menu(); break;
			} 
			case 8: {
				g = origin.restore(); break;
			}
		}
	}

	return 0;
}