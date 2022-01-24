// Вариант 3: фигура - прямоугольник, коллекция - стек, аллокатор - стек
#include <iostream>
#include <cstdlib>
#include <memory>
#include <utility>
#include <stack>
#include <exception>
#include <algorithm>
#include <list>
#include <functional>
#include <cassert>
// Класс прямоугольник
template <class T>
struct Rectangle{
	std::pair<T,T> a; // левая нижняя точка прямоугольника
	T w, h; // длина и ширина
    Rectangle() : a(std::make_pair(0,0)), h(0), w(0) { }
	Rectangle(std::pair<T,T> a_, T w_, T h_)
		: a(a_), w(w_), h(h_) { }
    // Печать прямоугольника
	friend std::ostream& operator<<(std::ostream &out, const Rectangle &r){
		out << "A (" << r.a.first << "," << r.a.second << "); ";
		out << "B (" << r.a.first << "," << r.a.second + r.h << "); ";
		out << "C (" << r.a.first + r.w << "," << r.a.second + r.h << "); ";
		out << "D (" << r.a.first + r.w << "," << r.a.second << ")";
		return out;
	}
    T square() { return w * h; } // Площадь прямоугольника
};
// чтение прямоугольника
template <class T>
void read(T &t){
    while(true){
        std::cin >> t.a.first; std::cin >> t.a.second; std::cin >> t.w; std::cin >> t.h;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "You can only write " << typeid(t.w).name() << " type. Input X Y W H again" << std::endl;
        }
        else if(t.w < 0 || t.h < 0){
            std::cin.clear();
            std::cin.ignore(32767,'\n');
            std::cout << "DIAG >= 0. Input X Y W H again" << std::endl;
        }
        else{
            std::cin.ignore(32767,'\n'); break;
        }
    }       
}
// Аллокатор, динамическая коллекция - стек
template <typename T, size_t BLOCK_COUNT>
class allocator{
private:
unsigned char* used_blocks;
std::stack<void*> free_blocks;
int count;
public:
	using value_type = T;
	using pointer = T * ;
    using const_pointer = const T*;
    using size_type = std::size_t;
    allocator(){ // выделение BLOCK_COUNT*sizeof(T) памяти 
    	static_assert(BLOCK_COUNT > 0);
    	used_blocks = (unsigned char*) std::malloc(sizeof(T) * BLOCK_COUNT);
    	for(size_t i = 0; i < BLOCK_COUNT; ++i){
    		void *k = std::malloc(BLOCK_COUNT);
    		k = used_blocks + i * sizeof(T);
    		free_blocks.push(k); k = 0;
    		std::free(k);
    	}
    	count = BLOCK_COUNT;
    }	
	template<typename U>
            struct rebind {
                using other = allocator<U,BLOCK_COUNT>;
            };
    ~allocator(){ // освобождение занаятой памяти
    	while(!free_blocks.empty()){
    		free_blocks.top() = 0;
    		std::free(free_blocks.top());
    		free_blocks.pop();
    	}
        std::free(used_blocks); 
    }
    T* allocate(size_t n){ // возвращает со стека свободный блок памяти
    	T* rez = nullptr;
    	if(count > 0){
    		rez = (T*)free_blocks.top();
    		free_blocks.top() = 0; std::free(free_blocks.top());
    		free_blocks.pop(); count--;
    		return rez;
    	}
    	return nullptr;
    }
    void deallocate(T* pointer,  size_t ) { // возвращает обратно на стек блок памяти
    	free_blocks.push(pointer);
    	count++;
    }
	template<typename U, typename ...Args>
     void construct(U *p, Args &&...args) {
     	new (p) U(std::forward<Args>(args)...);
     }
    void destroy(pointer p) {
        p->~T();
    } 
};
// Класс стек
template <class T, class ALLOCATOR>
class Stack{
	struct Node{ // Класс узел стека	
		using allocator_type = typename ALLOCATOR::template rebind<Node>::other;
        std::shared_ptr<Node> next; // указатель на хвост стека
		T value; // поле данных
        Node(T val, std::shared_ptr<Node> ptr){
            value = val;
            next = ptr;
        }
        void erase(Node &prev,int index){ // удаление узла по индексу 
            if(index == 0){
                prev.next = std::move(next); return;
            } 
            else if(next) return next->erase(*this,--index);
            throw std::logic_error("Out of bounds"); 
        }
		static allocator_type get_allocator(){ // инициализация
            static allocator_type allocator;
            return allocator;
        }
        void *operator new(size_t size){ // перегрузка new 
            return get_allocator().allocate(sizeof(Node));
        }
        void operator delete(void *p){ // перегрузка delete
            get_allocator().destroy((Node*)p);
            get_allocator().deallocate((Node *)p, sizeof(Node));
        }
    }; 
	std::shared_ptr<Node> head; // голова
    int size; // размер
public:
    Stack(){ size = 0; head = nullptr; }
	void push(const T &val){ // добавить элемент в стек         
        std::shared_ptr<Node> ptr = std::make_shared<Node>(val,nullptr);
        if(!head) head = ptr;
        else{
            ptr->next = head; head = ptr;
        }
        size++;
	}
	void pop(){ // удаление головы из стека
		if(size){
			head = head->next; size--;
            return;
		}
        throw std::logic_error("Size = 0");
	}
    void delete_index(int index){ // удаление элемента по индексу
        if(size < index + 1 || index < 0) throw std::logic_error("Out of bounds");
        if(index == 0) head = std::move(head->next);
        else if(head->next) head->next->erase(*head, --index);
        size--;
    }
	T top(){ // голова стека
        if(size) return head->value;
        throw std::logic_error("Size = 0");
	}
    void print(std::ostream &os){ // печать стека 
        std::function<void(std::shared_ptr<Node>)> print_node = [&](std::shared_ptr<Node> node) {
            os << node->value << std::endl;
            if(node->next) print_node(node->next);
        };
        if(size) print_node(head);
        else throw std::logic_error("Size = 0");
    }
    int size_() { return size; } // размер стека
};
// меню 
void print_menu()
{
    std::cout << "MENU\n";
    std::cout << "1. Push figure\n"
         << "2. Pop figure\n"
         << "3. Delete figure [i]\n"
         << "4. Top figure\n"
         << "5. Print stack\n"
         << "6. Size of stack\n"
         << "7. Print menu\n"
         << "8. std::list for check\n"
         << "0. Exit\n" << std::endl;
}
int main()
{
    using val_t = Rectangle<int>; // для координат, длины и ширины используется тип int 
    val_t r;
    Stack<val_t, allocator<val_t,50>> st;
    int j, i;
    print_menu();
    while(j){
        std::cout << "\nInput variant:\n";
        std::cin >> j;
        switch(j){
            case 1:{ // добавление элемента в стек
                if(st.size_() < 50){
                    std::cout << "\nInput lower left coordinate, width and length of rectangle: X Y W H:\n";
                    read(r); st.push(r);
                }
                else std::cout << "Size = n" << std::endl;
                break;
            }
            case 2:{
                try{ // удаление элемента из стека
                    st.pop();
                } catch(std::logic_error &exp){
                    std::cerr << exp.what() << std::endl;
                }
                break;
            }
            case 3:{ // удаление по индексу
                std::cout << "\nInput number of figure:\n"; std::cin >> i;
                try{
                    st.delete_index(i);
                }catch(std::logic_error &exp){
                    std::cerr << exp.what() << std::endl; 
                }   
                break;
            }
            case 4:{ // верхушка стека
                try{
                    std::cout << "\nTop of stack: " << st.top() << std::endl;
                }catch(std::logic_error &exp){
                    std::cerr << exp.what() << std::endl; 
                }
                
                break;
            }
            case 5:{
                try{ // печать стека
                    std::cout << "Stack:" << std::endl;
                    st.print(std::cout);
                } catch(std::logic_error &exp){
                    std::cerr << exp.what() << std::endl; 
                }
                break;
            }
            case 6:{ // размер стека
                std::cout << "Size of stack: " << st.size_() << std::endl;
                break;
            }
            case 7:{ // печать меню
                print_menu(); break;
            }
            case 8:{ // Проверка совместимости на примере с std::list - добавление 5 фигур, их печать и уничтожение
                std::cout << "std::list of rectangles, size of list = 5" << std::endl;
                std::list<val_t,allocator<val_t,5>> lst;
                for(int i = 0; i < 5; ++i){
                    std::cout << "\nInput lower left coordinate, width and length of rectangle: X Y W H:\n";
                    read(r);
                    lst.push_back(r);
                }
                std::cout << "std::list:" << std::endl;
                std::for_each(lst.begin(),lst.end(),[](auto &elem){
                        std::cout << elem << std::endl;
                });
                for(int i = 0; i < 5; ++i){
                    lst.erase(lst.begin());
                }
            }
            case 0: break;
            default: std::cout << "\nInput 0-9" <<std::endl; break;
        }
    }
	return 0;
}