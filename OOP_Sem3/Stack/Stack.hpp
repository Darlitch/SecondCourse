#include <list>

template <typename T>
class Stack {
   public:
    Stack();
    ~Stack() noexcept;
    bool Empty() const noexcept;
    void Push(T& item) noexcept;
    T Pop() noexcept;
    T Top() const noexcept;  // поп, обратно пуш и возвращение элемента
    std::size_t Size() const noexcept;

   private:
    size_t size_;
    size_t capacity_;
    T* stackArr_;
    void Resize();
};

template <typename T>
Stack<T>::Stack() : size_(0), capacity_(2) {
    stackArr_ = new T[capacity_];
}

template <typename T>
Stack<T>::~Stack() noexcept {
    delete[] stackArr_;
}

template <typename T>
void Stack<T>::Resize() {
    size_t capacity = capacity_ * 2;
    T* arr = new T[capacity];
    memcpy(arr, stackArr_, capacity_ * sizeof(T));
    capacity_ = capacity;
    delete[] stackArr_;
    stackArr_ = arr;
}

template <typename T>
bool Stack<T>::Empty() const noexcept {
    if (size_ == 0) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
std::size_t Stack<T>::Size() const noexcept {
    return size_;
}

template <typename T>
void Stack<T>::Push(T& item) noexcept {
    if (size_ == capacity_) {
        Resize();
    }
    stackArr_[size_] = item;
    size_++;
}

template <typename T>
T Stack<T>::Pop() noexcept {
    T temp = stackArr_[size_ - 1];
    size_--;
    return temp;
}

template <typename T>
T Stack<T>::Top() const noexcept {
    return stackArr_[size_ - 1];
}
