#include <list>

template <typename T>
class Stack {
   public:
    Stack() {}
    ~Stack();
    bool Empty();
    void Push(T& item);
    T Pop();
    T Top();  // поп, обратно пуш и возвращение элемента
    std::size_t Size();

   private:
    std::list<T> stack;
};

template <typename T>
Stack<T>::~Stack() {
    stack.clear();
}

template <typename T>
bool Stack<T>::Empty() {
    if (stack.size() == 0) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
std::size_t Stack<T>::Size() {
    return stack.size();
}

template <typename T>
void Stack<T>::Push(T& item) {
    stack.push_front(item);
}

template <typename T>
T Stack<T>::Pop() {
    T temp = stack.front();
    stack.pop_front();
    return temp;
}

template <typename T>
T Stack<T>::Top() {
    return stack.front();
}
