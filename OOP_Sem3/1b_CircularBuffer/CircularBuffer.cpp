#include "CircularBuffer.hpp"

#include <algorithm>
#include <stdexcept>

Buffer::CircularBuffer::CircularBuffer() : sizeBuff{0}, capacityBuff{50}, begin{0}, end{0} {
    buffer = new value_type[capacityBuff];
}

Buffer::CircularBuffer::~CircularBuffer() {
    delete[] buffer;
}

Buffer::CircularBuffer::CircularBuffer(const CircularBuffer& cb) {
    sizeBuff = cb.size();
    capacityBuff = cb.capacity();
    buffer = new value_type[capacityBuff];
    begin = cb.begin;
    end = cb.end;
    std::copy_n(cb.buffer, sizeBuff, buffer);
}

// Конструирует буфер заданной ёмкости.
Buffer::CircularBuffer::CircularBuffer(std::size_t capacity) : sizeBuff{0}, capacityBuff{capacity}, begin{0}, end{0} {
    if (capacityBuff < 2) {
        capacityBuff = 2;
    }
    buffer = new value_type[capacityBuff];
}

// Конструирует буфер заданной ёмкости, целиком заполняет его элементом elem.
Buffer::CircularBuffer::CircularBuffer(std::size_t capacity, const value_type& elem) : sizeBuff{capacity}, capacityBuff{capacity}, begin{0}, end{capacity - 1} {
    if (capacityBuff < 2) {
        capacityBuff = 2;
    }
    buffer = new value_type[capacityBuff];
    std::fill(buffer, buffer + capacityBuff, elem);
}

// Доступ по индексу. Не проверяют правильность индекса.
value_type& Buffer::CircularBuffer::operator[](std::size_t i) {
    return buffer[(i + begin) % capacityBuff];
}
const value_type& Buffer::CircularBuffer::operator[](std::size_t i) const {
    return buffer[(i + begin) % capacityBuff];
}

// Доступ по индексу. Методы бросают исключение в случае неверного индекса.
value_type& Buffer::CircularBuffer::at(std::size_t i) {
    if (i >= capacityBuff)
        throw std::out_of_range("Invalid index");  // ДОРАБОТАТЬ ИСКЛЮЧЕНИЯ
    return buffer[(i + begin) % capacityBuff];
}
const value_type& Buffer::CircularBuffer::at(std::size_t i) const {
    if (i >= capacityBuff)\
        throw std::out_of_range("Invalid index");  // ДОРАБОТАТЬ ИСКЛЮЧЕНИЯ
    return buffer[(i + begin) % capacityBuff];
}

value_type& Buffer::CircularBuffer::front() {
    if (empty())
        throw std::out_of_range("Buffer empty");
    return buffer[begin];
}  // Ссылка на первый элемент.
value_type& Buffer::CircularBuffer::back() {
    if (empty())
        throw std::out_of_range("Buffer empty");
    return buffer[end];
}  // Ссылка на последний элемент.
const value_type& Buffer::CircularBuffer::front() const {
    if (empty())
        throw std::out_of_range("Buffer empty");
    return buffer[begin];
}
const value_type& Buffer::CircularBuffer::back() const {
    if (empty())
        throw std::out_of_range("Buffer empty");
    return buffer[end];
}

// Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
// переместится в начало аллоцированной памяти. Возвращает указатель
// на первый элемент.
value_type* Buffer::CircularBuffer::linearize() {
    std::rotate(buffer, buffer + begin, buffer + capacityBuff);
    begin = 0;
    end = sizeBuff - 1;
    return buffer;
}
// Проверяет, является ли буфер линеаризованным.
bool Buffer::CircularBuffer::is_linearized() const {
    if (begin == 0) {
        return true;
    } else {
        return false;
    }
}
// Сдвигает буфер так, что по нулевому индексу окажется элемент
// с индексом new_begin.
void Buffer::CircularBuffer::rotate(std::size_t new_begin) {
    begin = (begin + new_begin) % capacityBuff;
    end = (end + new_begin) % capacityBuff;
}

// Количество элементов, хранящихся в буфере.
std::size_t Buffer::CircularBuffer::size() const {
    return sizeBuff;
}

bool Buffer::CircularBuffer::empty() const {
    if (sizeBuff == 0) {
        return true;
    } else {
        return false;
    }
}
// true, если size() == capacity().
bool Buffer::CircularBuffer::full() const {
    if (sizeBuff == capacityBuff) {
        return true;
    } else {
        return false;
    }
}
// Количество свободных ячеек в буфере.
std::size_t Buffer::CircularBuffer::reserve() const {
    return capacityBuff - sizeBuff;
}
// ёмкость буфера
std::size_t Buffer::CircularBuffer::capacity() const {
    return capacityBuff;
}

void Buffer::CircularBuffer::set_capacity(std::size_t new_capacity) {
    if (!is_linearized()) {
        linearize();
    }
    if (new_capacity < 2) {
        new_capacity = 2;
    }
    value_type* newBuff = new value_type[new_capacity];
    std::copy_n(buffer, sizeBuff, newBuff);
    capacityBuff = new_capacity;
    delete[] buffer;
    buffer = newBuff;
    if (sizeBuff > capacityBuff) {
        sizeBuff = capacityBuff;
    }
}
// Изменяет размер буфера.
// В случае расширения, новые элементы заполняются элементом item.
void Buffer::CircularBuffer::resize(std::size_t new_size, const value_type& item) {
    if (new_size > capacityBuff) {
        set_capacity(new_size);
    } else if (!is_linearized()) {
        linearize();
    }
    if (new_size > sizeBuff) {
        for (std::size_t i = ++end; i < new_size; ++i) {
            buffer[i] = item;
        }
    }
    end = new_size - 1;
    sizeBuff = new_size;
}
// Оператор присваивания.
Buffer::CircularBuffer& Buffer::CircularBuffer::operator=(const CircularBuffer& cb) {
    delete[] buffer;
    buffer = new value_type[cb.capacityBuff];
    std::copy_n(cb.buffer, cb.sizeBuff, buffer);
    sizeBuff = cb.sizeBuff;
    capacityBuff = cb.capacityBuff;
    begin = cb.begin;
    end = cb.end;
    return *this;
}
// Обменивает содержимое буфера с буфером cb.
void Buffer::CircularBuffer::swap(CircularBuffer& cb) {
    std::swap(buffer, cb.buffer);
    std::swap(begin, cb.begin);
    std::swap(end, cb.end);
    std::swap(sizeBuff, cb.sizeBuff);
    std::swap(capacityBuff, cb.capacityBuff);
}

// Добавляет элемент в конец буфера.
// Если текущий размер буфера равен его ёмкости, то переписывается
// первый элемент буфера (т.е., буфер закольцован).
void Buffer::CircularBuffer::push_back(const value_type& item) {
    if (sizeBuff != 0) {
        end = (end + 1) % capacityBuff;
        if (end == begin) {
            begin = (begin + 1) % capacityBuff;
        }
    }
    buffer[end] = item;
    if (sizeBuff < capacityBuff) {
        sizeBuff++;
    }
}
// Добавляет новый элемент перед первым элементом буфера.
// Аналогично push_back, может переписать последний элемент буфера.
void Buffer::CircularBuffer::push_front(const value_type& item) {
    if (sizeBuff != 0) {
        begin = (begin + capacityBuff - 1) % capacityBuff;
        if (begin == end) {
            end = (end + capacityBuff - 1) % capacityBuff;
        }
    }
    buffer[begin] = item;
    if (sizeBuff < capacityBuff) {
        sizeBuff++;
    }
}
// удаляет последний элемент буфера.
void Buffer::CircularBuffer::pop_back() {
    end = (end + capacityBuff - 1) % capacityBuff;
    sizeBuff--;
}
// удаляет первый элемент буфера.
void Buffer::CircularBuffer::pop_front() {
    begin = (begin + 1) % capacityBuff;
    sizeBuff--;
}

// Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
void Buffer::CircularBuffer::insert(std::size_t pos, const value_type& item) {
    if (!is_linearized()) {
        linearize();
    }
    if (sizeBuff != capacityBuff) {
        end++;
        sizeBuff++;
    }
    buffer[end] = item;
    std::rotate(buffer + pos, buffer + end, buffer + end + 1);
}
// Удаляет элементы из буфера в интервале [first, last).
void Buffer::CircularBuffer::erase(std::size_t first, std::size_t last) {
    std::size_t shift = last - first + 1;
    if (!is_linearized()) {
        linearize();
    }
    std::rotate(buffer + first, buffer + last + 1, buffer + end + 1);
    end -= last - first + 1;
}
// Очищает буфер.
void Buffer::CircularBuffer::clear() {
    sizeBuff = 0;
    begin = 0;
    end = 0;
}

bool Buffer::operator==(const CircularBuffer& a, const CircularBuffer& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool Buffer::operator!=(const CircularBuffer& a, const CircularBuffer& b) {
    return !(a == b);
}
