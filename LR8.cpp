//  Вариант 8: восьмиугольник, треугольник и квадрат
#include <iostream>
#include <utility>
#include <memory>
#include <fstream>
#include <exception>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>
#include <random>
#include <string>
#include <algorithm>

// класс фигура
template <class T>
class Figure{
public:
	virtual void write(std::ostream &out) = 0; // печать в файл
	virtual void read(std::istream &in) = 0; //считывание 
    virtual void print(std::ostream &out) = 0; // печать на экран
};
// класс треугольник
template <class T>
class Triangle : public Figure<T> {
private:
	std::pair<T,T> center;
	T radius;
public:
	Triangle() : center(std::make_pair(0,0)), radius(0) {}
	Triangle(std::pair<T,T> center_, T radius_) 
		: center(center_), radius(radius_) { }
	void write(std::ostream &out) override{
		out << 0 << ' ' << center.first << ' '<< center.second << ' ' << radius << std::endl;
	}
	void read(std::istream &in) override{
		in >> center.first >> center.second >> radius;
	}
    void print(std::ostream &out) override {
        out << "Triangle:";
        for(int i = 0; i < 3; ++i){
            double angle = 2*M_PI*i/3;
            out << "(" << radius*cos(angle) + center.first << ", " << radius*sin(angle) + center.second << "); " ;
        }
        out << std::endl;   
    }
};
// класс квадрат
template <class T>
class Square : public Figure <T> {
private:
	std::pair<T,T> center;
	T radius;	
public:
	Square() : center(std::make_pair(0,0)), radius(0) {}
 	Square(std::pair<T,T> center_, T radius_) 
		: center(center_), radius(radius_) { }
 	void write(std::ostream &out) override{
		out << 1 << ' ' << center.first << ' ' << center.second << ' ' << radius << std::endl;
	}
	void read(std::istream &in) override{
		in >> center.first >> center.second >> radius;
	}
    void print(std::ostream &out) override {
        out << "Square:";
        for(int i = 0; i < 4; ++i){
            double angle = 2*M_PI*i/4;
            out << "(" << radius*cos(angle) + center.first << ", " << radius*sin(angle) + center.second << "); " ;
        }
        out << std::endl;   
    }
};
// класс восьмиугольник
template <class T>
class Octagon : public Figure<T> {
private:
	std::pair<T,T> center;
	T radius;
public:
	Octagon() : center(std::make_pair(0,0)), radius(0) {}
	Octagon(std::pair<T,T> center_, T radius_) 
		: center(center_), radius(radius_) { }
 	void write(std::ostream &out) override{
		out << 2 << ' ' << center.first << ' ' << center.second << ' ' << radius << std::endl;
	}
	void read(std::istream &in) override{
		in >> center.first >> center.second >> radius;
	}
    void print(std::ostream &out) override {
        out << "Octagon:";
        for(int i = 0; i < 8; ++i){
            double angle = 2*M_PI*i/8;
            out << "(" << radius*cos(angle) + center.first << ", " << radius*sin(angle) + center.second << "); " ;
        }
        out << std::endl;   
    }
};
// код события
enum class EventCode {
    cnt = 0, // продолжать выполнение
    print_figure = 1, // печать фигур
    quit = 2, // выход
    start = 3
};
// хранение кода события и фигуры
template <class T>
struct Event {
    EventCode code;
    size_t data;
    std::shared_ptr<Figure<T>> figures;
};
// класс сервер
template <class MESSAGE_T, class T>
class Server{
public: // тип лямбд для печати вектора на экран и в файл
    using subscriber_t = std::function<void(std::vector<MESSAGE_T> &)>;
    static Server &get() {
       static Server instance;
       return instance;
    }
    void register_subscriber(subscriber_t subs) { // добавление обработчиков в массив
        subscribers.push_back(subs);
    }
    void publish(const Event<T> &ev) { // добавление событий в очередь
        std::lock_guard<std::mutex> lck(mtx); 
        event_queue.push(ev);
    }
    void run() { // обработка события
    	bool cnt = true;
        while (cnt) { 
            if (!event_queue.empty()) {
                std::lock_guard<std::mutex> lck(mtx); 
                auto val = event_queue.front();
                event_queue.pop();
                if(val.figures) {
                    vector.push_back(val.figures); // добавление элемента в буфер
                    if(vector.size() == vec_size) // очистка буфера
                        for (auto subs : subscribers) 
                            try{
                                subs(vector);
                            }catch(std::exception &ex){
                                std::cerr << "Exception in subscriber:" << ex.what() << std::endl;
                            }
                }
                switch(val.code){
                    case EventCode::start : vec_size = val.data; break;
                	case EventCode::cnt : break; // продолжение выполнения
                	case EventCode::print_figure : { // печать фигур
                		for (auto subs : subscribers) 
                 			try{
                    			subs(vector);
                 			}catch(std::exception &ex){
                     		std::cerr << "Exception in subscriber:" << ex.what() << std::endl;
                 		}
                		break;
                	}
                	case EventCode::quit :{ // выход
                        cnt = false; break;
                    } 
                    default : break;
                }           
            }
            else {
            	 std::this_thread::yield(); // передаем управление
            }
        }
    }

private:
    std::vector<subscriber_t> subscribers;
    std::queue<Event<T>> event_queue;
    std::mutex mtx;
    std::vector<std::shared_ptr<Figure<T>>> vector;
    size_t vec_size;
    Server() {};
};
// для определения при вводе фигуры
enum class FigureType{
	Triangle = 0,
	Square = 1,
	Octagon = 2
};
// ввод фигуры
template <class T>
struct Factory{
	static std::shared_ptr<Figure<T>> create(FigureType t, T a = 0, T b = 0, T radius = 0){
		switch(t){
			case FigureType::Triangle:
				return std::make_shared<Triangle<T>>(std::make_pair(a,b), radius);
			case FigureType::Square:
				return std::make_shared<Square<T>>(std::make_pair(a,b), radius);
			case FigureType::Octagon:
				return std::make_shared<Octagon<T>>(std::make_pair(a,b), radius);
		}
		return nullptr; // если некорректный ввод
	}
};
// генерация имени файла
std::string random_string()
{
     std::string str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
     std::random_device rd;
     std::mt19937 generator(rd());
     std::shuffle(str.begin(), str.end(), generator);
     return str.substr(0,10);      
}
// печать меню
void print_menu(){
	std::cout << "\nMENU\n";
	std::cout << "1. Add figure\n"
		 << "2. Print figures\n"
		 << "3. Print menu\n"
		 << "0. Exit\n" << std::endl;
}

int main(int argc, char *argv[])
{
    size_t size; 
    int type;
	if(argc == 2){
		size = atoi(argv[1]);
	}
	else {
		std::cout << "Input buffer size: " << std::endl;
		std::cin >> size;
	}
    // печать на экран вектора
	Server<std::shared_ptr<Figure<int>>,int>::get().register_subscriber([size](std::vector<std::shared_ptr<Figure<int>>> &val) {
        if(val.size() < size){
            std::cout << "\nBuffer" << std::endl;
        	for(auto value : val)
        		value->print(std::cout);
            std::cout.flush();
        }
    });
    // печать в файл вектора
   	Server<std::shared_ptr<Figure<int>>,int>::get().register_subscriber([size](std::vector<std::shared_ptr<Figure<int>>> &val) {
        if(val.size() == size){
        	std::fstream file;
        	std::string name;
        	while(true){
        		name = random_string();
        		file.open(name + ".txt", std::ios::in);
        		if(file.is_open()) {
        			file.close();
        			continue;
        		}
        		break;
        	}
        	file.open(name + ".txt", std::ios::out);
            std::cout << "\nBuffer" << std::endl;
            for(auto value : val){
                value->print(std::cout);
        		value->write(file);
            }
        	std::cout << "\nBuffer in file " << name + ".txt" << std::endl;
            std::cout.flush();
        	file.close();
	       	val.clear();
        }
    });
    // второй поток 
   	std::thread th([]() {
        Server<std::shared_ptr<Figure<int>>,int>::get().run();
    });

    std::shared_ptr<Figure<int>> figure;
    print_menu();
    int value = 1;
    Event<int> event;
    event.code = EventCode::start;
    event.data = size;
    Server<std::shared_ptr<Figure<int>>,int>::get().publish(event);

    while(true){
    	std::cout << "Input value\n";
    	std::cin >> value;
    	if(value == 0) { // выход
    		event.code = EventCode::quit;
    		event.figures = nullptr;
    		Server<std::shared_ptr<Figure<int>>,int>::get().publish(event);
    		break;
    	}
    	switch(value){
    		case 1:{ // вставка фигуры
    			std::cout << "Input type of figure: 0 - Triangle, 1 - Square, 2 - Octagon" << std::endl;
    			std::cin >> type;
    			std::shared_ptr<Figure<int>> figure = Factory<int>::create((FigureType)type);
    			std::cout << "Input coords of center and radius of figure" << std::endl;
    			figure->read(std::cin);
                event.code = EventCode::cnt;
                event.figures = figure;
    			Server<std::shared_ptr<Figure<int>>,int>::get().publish(event);
    			break;
    		}
    		case 2:{ // печать фигуры
    			event.code = EventCode::print_figure;
    			event.figures = nullptr;
    			Server<std::shared_ptr<Figure<int>>,int>::get().publish(event);
    			break;
    		}
    		case 3:{
    			print_menu();
    			break;
    		}
    		default: break;
    	}
       std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    th.join();

    return 0;
}
 