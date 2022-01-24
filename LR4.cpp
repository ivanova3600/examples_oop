#include <iostream>
#include <utility>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <cmath>

// Класс правильный пятиугольник, поля - это центр и радиус описанной окружности
template <class T> struct Pentagon{
	using type = T;
	std::pair<T,T> center;
	T radius;
	bool pen = true;
};

template <class T> struct Hexagon{
	using type = T;
	std::pair<T,T> center;
	T radius;
	bool hex = true;
};

template <class T> struct Octagon{
	using type = T;
	std::pair<T,T> center;
	T radius;
	bool oct = true;
};

// Печать на экран std::tuple
template<std::size_t size = 0, typename... Tp>
typename std::enable_if<size == sizeof...(Tp), void>::type
  print(std::tuple<Tp...> &t) { }

template<std::size_t size = 0, typename... Tp>
typename std::enable_if<size < sizeof...(Tp), void>::type
  print(std::tuple<Tp...>& t)
  {
    auto i = std::get<size>(t);
    print_figure(i);
    print<size + 1, Tp...>(t);
  }

// Печать Pentagon
template<class T>
typename std::enable_if<(sizeof(T::pen) > 0), void>::type
print_figure(T &path){
	std::cout << "Pentagon: R = " << path.radius << std::endl;
	for(int i = 0; i < 5; ++i){
		double angle = 2*M_PI*i/5;
		std::cout << "X" << i + 1 << ": " << path.radius*cos(angle) + path.center.first;
		std::cout << ", Y" << i + 1 << ": " << path.radius*sin(angle) + path.center.second << std::endl;
	}
}

// печать Hexagon
template<class T>
typename std::enable_if<(sizeof(T::hex) > 0), void>::type
print_figure(T &path){
	std::cout << "Hexagon: R = " << path.radius << std::endl;
	for(int i = 0; i < 6; ++i){
		double angle = 2*M_PI*i/6;
		std::cout << "X" << i + 1 << ": " << path.radius*cos(angle) + path.center.first;
		std::cout << ", Y" << i + 1 << ": " << path.radius*sin(angle) + path.center.second << std::endl;
	}
}

//печать Octagon
template<class T>
typename std::enable_if<(sizeof(T::oct) > 0), void>::type
print_figure(T &path){
	std::cout << "Octagon: R = " << path.radius << std::endl;
	for(int i = 0; i < 8; ++i){
		double angle = 2*M_PI*i/8;
		std::cout << "X" << i + 1 << ": " << path.radius*cos(angle) + path.center.first;
		std::cout << ", Y" << i + 1 << ": " << path.radius*sin(angle) + path.center.second << std::endl;
	}
}

//Вычисление площади пятиугольника
template<class T>
typename std::enable_if<(sizeof(T::pen) > 0), double>::type
calcSquare(T &path){
	return static_cast<double>(5 / 4 * path.radius * path.radius * sin(2 * M_PI / 5));
}

//  Площадь шестиугольника
template<class T>
typename std::enable_if<(sizeof(T::hex) > 0), double>::type
calcSquare(T &path){
	return static_cast<double>(6 / 4 * path.radius * path.radius * sin(2 * M_PI / 6));
}

// Площадь восьмиугольника
template<class T>
typename std::enable_if<(sizeof(T::oct) > 0), double>::type
calcSquare(T &path){
	return static_cast<double>(5 / 8 * path.radius * path.radius * sin(2 * M_PI / 8));
}

//Вычисление площади tuple
template<std::size_t size = 0, typename... Tp>
typename std::enable_if<size == sizeof...(Tp), double>::type
  square_tuple(std::tuple<Tp...> &t) 
  { 
  	return 0;
  }

template<std::size_t size = 0, typename... Tp>
typename std::enable_if<size < sizeof...(Tp), double>::type
  square_tuple(std::tuple<Tp...>& t)
  {
    auto i = std::get<size>(t);
    double value = calcSquare(i);
    return value + square_tuple<size + 1, Tp...>(t);
  }

// Ввод фигур
template<class T>
 void read(T &t){
  	while(true){
  		std::cin >> t.center.first; std::cin >> t.center.second; std::cin >> t.radius;
  		if(std::cin.fail()){
  			std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "You can only write " << typeid(t.radius).name() << " type. Input X Y R again" << std::endl;
  		}
  		else if(t.radius < 0){
  			std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "Radius >= 0. Input X Y R again" << std::endl;
  		}
  		else{
  			std::cin.ignore(32767,'\n'); break;
  		}
  	} 		
  }

// Печать меню
void print_menu()
{
	std::cout << "\nMENU\n";
	std::cout << "1. Print figures\n"
		 << "2. Print square of tuple\n"
		 << "3. Print menu\n0. Exit\n" << std::endl;
}


int main()
{
	std::cout << "You need input center and radius of 2 pentagons, 2 hexagons and 2 octagons: X Y R" << std::endl;
	Pentagon<int> p1;
	std::cout << "Input pentagon #1 (int coords and radius)\n"; read(p1);
	Pentagon<double> p2;
	std::cout << "Input pentagon #2(double coords and radius)\n"; read(p2);
	Hexagon<int> h1;
	std::cout << "Input hexagon #1 (int coords and radius)\n"; read(h1);
	Hexagon<double> h2;
	std::cout << "Input hexagon #2(double coords and radius)\n"; read(h2);
	Octagon<int> o1;
	std::cout << "Input octagon #1 (int coords and radius)\n"; read(o1);
	Octagon<int> o2;
	std::cout << "Input octagon #2(double coords and radius)\n"; read(o2);
	

	std::tuple<decltype(p1), decltype(p2), decltype(h1), decltype(h2), decltype(o1), decltype(o2)> T(p1, p2, h1, h2, o1, o2);

 	int k = 1;
 	print_menu();
 	while(k){
 		std::cout << "Input variant" << std::endl;
 		std::cin >> k;
 		switch(k){
 			case 1:{
 				std::cout << std::endl;
 				print(T);
 				std::cout << std::endl;
 				break;
 			}
 			case 2:{
 				std::cout << "Square of tuple " << square_tuple(T)  << '\n' << std::endl;
 				break;
 			}
 			case 3:{
 				print_menu();
 				break;
 			}
 			default: break;
 		}
 	}

 	return 0;
}