#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <iostream>

using value_type = char;

class CircularBuffer {
    /*... реализация ... */
   public:
    CircularBuffer();
    ~CircularBuffer();
    CircularBuffer(const CircularBuffer& cb);

    // Конструирует буфер заданной ёмкости.
    explicit CircularBuffer(std::size_t capacity);
    // Конструирует буфер заданной ёмкости, целиком заполняет его элементом elem.
    CircularBuffer(std::size_t capacity, const value_type& elem);

    // Доступ по индексу. Не проверяют правильность индекса.
    value_type& operator[](std::size_t i);
    const value_type& operator[](std::size_t i) const;

    // Доступ по индексу. Методы бросают исключение в случае неверного индекса.
    value_type& at(std::size_t i);
    const value_type& at(std::size_t i) const;

    value_type& front();  // Ссылка на первый элемент.
    value_type& back();   // Ссылка на последний элемент.
    const value_type& front() const;
    const value_type& back() const;

    // Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
    // переместится в начало аллоцированной памяти. Возвращает указатель
    // на первый элемент.
    value_type* linearize();
    // Проверяет, является ли буфер линеаризованным.
    bool is_linearized() const;
    // Сдвигает буфер так, что по нулевому индексу окажется элемент
    // с индексом new_begin.
    void rotate(std::size_t new_begin);
    // Количество элементов, хранящихся в буфере.
    std::size_t size() const;
    bool empty() const;
    // true, если size() == capacity().
    bool full() const;
    // Количество свободных ячеек в буфере.
    std::size_t reserve() const;
    // ёмкость буфера
    std::size_t capacity() const;

    void set_capacity(std::size_t new_capacity);
    // Изменяет размер буфера.
    // В случае расширения, новые элементы заполняются элементом item.
    void resize(std::size_t new_size, const value_type& item = value_type());
    // Оператор присваивания.
    CircularBuffer& operator=(const CircularBuffer& cb);
    // Обменивает содержимое буфера с буфером cb.
    void swap(CircularBuffer& cb);

    // Добавляет элемент в конец буфера.
    // Если текущий размер буфера равен его ёмкости, то переписывается
    // первый элемент буфера (т.е., буфер закольцован).
    void push_back(const value_type& item = value_type());
    // Добавляет новый элемент перед первым элементом буфера.
    // Аналогично push_back, может переписать последний элемент буфера.
    void push_front(const value_type& item = value_type());
    // удаляет последний элемент буфера.
    void pop_back();
    // удаляет первый элемент буфера.
    void pop_front();

    // Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
    void insert(std::size_t pos, const value_type& item = value_type());
    // Удаляет элементы из буфера в интервале [first, last).
    void erase(std::size_t first, std::size_t last);
    // Очищает буфер.
    void clear();

   private:
    value_type* buffer;
    std::size_t sizeBuff;
    std::size_t capacityBuff;
    std::size_t first = 0;
    std::size_t end = 0;
};

bool operator==(const CircularBuffer& a, const CircularBuffer& b);
bool operator!=(const CircularBuffer& a, const CircularBuffer& b);

#endif
