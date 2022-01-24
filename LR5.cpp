// Вариант 13: ромб и список
#include <iostream>
#include <utility>
#include <memory>
#include <exception>
#include <iterator>
#include <initializer_list>
#include <algorithm>
// Ромб: диагонали параллельны осям координат, задан через координаты пересечения диагоналей и длины диагоналей
template <class T>
struct Rhombus{
	using vertex_t = std::pair<T,T>;
	vertex_t a;
	T diag_x;
	T diag_y;
	Rhombus() : a(std::make_pair(0,0)), diag_x(0), diag_y(0) { }
	Rhombus(const vertex_t &aa, const T &x, const T &y) 
		: a(aa), diag_x(x), diag_y(y) { }
	Rhombus(const Rhombus &r) {
		a = r.a; diag_x = r.diag_x; diag_y = r.diag_y;
	}
	Rhombus& operator=(const Rhombus &r){
		a = r.a; diag_x = r.diag_x; diag_y = r.diag_y;
		return *this;
	}
};
// Печать ромба
template <class T>
std::ostream& operator<<(std::ostream &out, const Rhombus<T> &r){
	out << "A [" << static_cast<double>(r.a.first) - (static_cast<double>(r.diag_x) / 2) << "," << r.a.second << "]; ";
	out << "B [" << r.a.first << "," << static_cast<double>(r.a.second) + (static_cast<double>(r.diag_y) / 2) << "]; ";
	out << "C [" << static_cast<double>(r.a.first) + (static_cast<double>(r.diag_x) / 2)<< ',' << r.a.second << "]; ";
	out << "D [" << r.a.first << "," << static_cast<double>(r.a.second) - (static_cast<double>(r.diag_y) / 2) << "]";
	return out;
}
// Узел списка: ссылка на следующий элемент и поле
template <class T>
struct Node{
	std::shared_ptr<Node<T>> next;
	T value;
	Node(T &vv){
		value = vv; next = nullptr;
	}   
	Node(T &vv,std::shared_ptr<Node<T>> ptr){
		value = vv; next = ptr;
	} 
	void erase(Node &prev,int index){ // удаление узла по индексу 
        if(index == 0){
            prev.next = std::move(next); return;
        } 
        else if(next) return next->erase(*this,--index);
        throw std::logic_error("Out of bounds"); 
    }
	void insert(Node &prev,int index,T& value){ // вставка узла по индесу 
        if(index == 0) {
        	std::shared_ptr<Node<T>> ptr{new Node<T>{value}};
        	ptr->next = prev.next; prev.next = std::move(ptr);
            return;
        } 
        else if(next) return next->insert(*this,--index,value);
        throw std::logic_error("Out of bounds"); 
    }
};
// Список: ссылка на голову списка
template <class T>
class List{
private:
	std::shared_ptr<Node<T>> head;
public:
	using value_type = T;
	List() { }
	List(std::initializer_list<T> il){ // инициализация через список инициализаций
		for(auto val : il) pushBack(val);
	}
	class ListIterator{ // класс итератор для списка
	private:
		List &list;
		int index;
		friend class List;
	public: // для удобства
		using difference_type = int;
        using value_type = List::value_type;
        using reference = List::value_type& ;
        using pointer = List::value_type*;
        using iterator_category = std::forward_iterator_tag;
        ListIterator(List &l,int i) : list(l), index(i) { } 
        ListIterator& operator++(){ // перегрузка ++
        	++index; return *this;
        }
        reference operator*() { return list[index]; } // перегрузка *
        pointer operator->(){ return &list[index]; } // перегрузка -> 
        bool operator!=(const ListIterator& i){ // перегрузка !=
            if(index!=i.index) return true;
            if(&list!=&(i.list)) return true;
            return false;
        }
        bool operator==(const ListIterator& i){ // перегрузка ==
            if(index!=i.index) return false;
            if(&list!=&(i.list)) return false;
            return true;
        }
       	ListIterator& operator+(const int &i){ // перегрузка +
        	index += i; return *this;
        }     
        ListIterator& operator-(const int &i){ // перегрузка -
        	index -= i; return *this;
        } 
	};
	void pushBack(T &vv){ // вставка в конец списка
		if(!head) head.reset(new Node<T>{vv}); 
		else{
			std::shared_ptr<Node<T>> ptr = head;
			while(ptr->next) ptr = ptr->next;
			ptr->next.reset(new Node<T>{vv}); 
		}
	}  	
	int size(){ // размер списка
		if(!head) return 0;
		std::shared_ptr<Node<T>> ptr = head;
		int size = 1;
		while(ptr->next) {
			size++; ptr = ptr->next;
		} 
		return size;
	}
	T& operator[](int index){ // перегрузка оператора []
		if(!head) {
			throw std::logic_error("Out of bounds");
		}
		else{
			std::shared_ptr<Node<T>> ptr = head;
			while(ptr){
				if(index == 0) return ptr->value;
				index--; ptr = ptr->next;
			}
			throw std::logic_error("Out of bounds");
		}
	}
	ListIterator erase(ListIterator iter){ // удаление из списка по индексу
		if(size() < iter.index + 1) throw std::logic_error("Out of bounds");
		if(iter.index == 0) head = std::move(head->next);
		else{
			if(head->next) head->next->erase(*head, iter.index-1);
		}
		if(iter.index<size()) return iter;
        return ListIterator(*this,size());
	}
	ListIterator insert (ListIterator iter, T& value){ // вставка в список по индексу
		if(size() < iter.index + 1) throw std::logic_error("Out of bounds");
        if(iter.index==0){
        	std::shared_ptr<Node<T>> ptr{new Node<T>{value}};
        	ptr->next = head;
            head = std::move(ptr);
        } 
        else {
            if(head->next) head->next->insert(*head, iter.index-1, value);
        }
        if(iter.index<size()) return iter;
        return ListIterator(*this,size());
    } 
    ListIterator begin(){
        return ListIterator(*this,0);
    }
    ListIterator end(){
        return ListIterator(*this,size());
    }
};
// Чтение ромба
template <class T>
void read(T &t){
	while(true){
  		std::cin >> t.a.first; std::cin >> t.a.second; std::cin >> t.diag_x; std::cin >> t.diag_y;
  		if(std::cin.fail()){
  			std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "You can only write " << typeid(t.diag_x).name() << " type. Input X Y D_X D_Y again" << std::endl;
  		}
  		else if(t.diag_x < 0 || t.diag_y < 0){
  			std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "DIAG >= 0. Input X Y D_X D_Y again" << std::endl;
  		}
  		else{
  			std::cin.ignore(32767,'\n'); break;
  		}
  	} 		
}
// Печать меню
void print_menu()
{
	std::cout << "MENU\n";
	std::cout << "1. Push back figure\n"
		 << "2. Insert figure\n"
		 << "3. Delete figure\n"
		 << "4. Print figure[i]\n"
		 << "5. Print list\n"
		 << "6. Print figures which area < k\n"
		 << "7. Print menu\n"
		 << "0. Exit\n" << std::endl;
}

int main()
{	
	List<Rhombus<int>> lst; // целочисленные по условию 
	Rhombus<int> r; 
	double area;
	int i, j = 5; print_menu();
	while(j){
		std::cout << "\nInput variant:\n";
		std::cin >> j;
		switch(j){
			case 1:{
				std::cout << "\nInput coords of the intersection point of the diagonals and the length of the diagonals: X Y DIAG_X DIAG_Y:\n";
				read(r); lst.pushBack(r);
				break;
			}
			case 2:{
				std::cout << "\nInput coords of the intersection point of the diagonals and the length of the diagonals: X Y DIAG_X DIAG_Y:\n";
				read(r); 
				std::cout << "\nInput number of figure:\n"; std::cin >> i;
				try{
				lst.insert(lst.begin() + i, r);
				}catch(std::logic_error &exp){
					std::cerr << exp.what() << std::endl; 
				}
				break;
			}
			case 3:{
				std::cout << "\nInput number of figure:\n"; std::cin >> i;
				try{
					lst.erase(lst.begin() + i); 
				}catch(std::logic_error &exp){
					std::cerr << exp.what() << std::endl; 
				}	
				break;
			}
			case 4:{
				std::cout << "\nInput number of figure:\n"; std::cin >> i;
				try{
					std::cout << "\nFigure number " << i << ": " << lst[i] << std::endl;
				}catch(std::logic_error &exp){
					std::cerr << exp.what() << std::endl; 
				}
				
				break;
			}
			case 5:{
				std::cout << "\nList:" << std::endl;
				std::for_each(lst.begin(), lst.end(),[](auto &elem){
						std::cout << elem << std::endl;
					});
				break;
			}
			case 6:{
				std::cout << "\nInput the smallest possible area:\n";
				std::cin >> area;
				int count = std::count_if(lst.begin(),lst.end(),
					[area](auto &value) -> bool{
						std::cout << static_cast<double>(value.diag_x) * static_cast<double>(value.diag_y) / 2 << std::endl;
						return (static_cast<double>(value.diag_x) * static_cast<double>(value.diag_y) / 2) >= area;
					});
				std::cout << "\nAreas greater " << area << " are " <<  count << std::endl;
				break;
			}
			case 7:{
				print_menu(); break;
			}
			case 0: break;
			default: std::cout << "\nInput 0-7" <<std::endl; break;
	    }

	}
	return 0;
}
