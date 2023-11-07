#include "CircularBuffer.hpp"

#include <algorithm>
#include <stdexcept>

CircularBuffer::CircularBuffer() : sizeBuff{0}, capacityBuff{50}, first{0}, end{0} {
    buffer = new value_type[capacityBuff];
}

CircularBuffer::~CircularBuffer() {
    delete[] buffer;
}

CircularBuffer::CircularBuffer(const CircularBuffer& cb) {
    sizeBuff = cb.sizeBuff;
    capacityBuff = cb.capacityBuff;
    buffer = new value_type[capacityBuff];
    std::copy_n(cb.buffer, sizeBuff, buffer);
}

explicit CircularBuffer::CircularBuffer(std::size_t capacity) : sizeBuff{0}, capacityBuff{capacity} {
    buffer = new value_type[capacityBuff];
}

CircularBuffer::CircularBuffer(std::size_t capacity, const value_type& elem) : sizeBuff{capacity}, capacityBuff{capacity}, end{capacity - 1} {
    buffer = new value_type[capacityBuff];
    std::fill(buffer, buffer + capacityBuff - 1, elem);
}

// Доступ по индексу. Не проверяют правильность индекса.
value_type& CircularBuffer::operator[](std::size_t i) {
    return buffer[i];
}
const value_type& CircularBuffer::operator[](std::size_t i) const {
    return buffer[i];
}

// Доступ по индексу. Методы бросают исключение в случае неверного индекса.
value_type& CircularBuffer::at(std::size_t i) {
    if (i >= capacityBuff) throw std::out_of_range("Invalid index");  // ДОРАБОТАТЬ ИСКЛЮЧЕНИЯ
    return buffer[i];
}
const value_type& CircularBuffer::at(std::size_t i) const {
    if (i >= capacityBuff) throw std::out_of_range("Invalid index");  // ДОРАБОТАТЬ ИСКЛЮЧЕНИЯ
    return buffer[i];
}

value_type& CircularBuffer::front() {
    if (empty()) throw std::out_of_range("Buffer empty");
    return buffer[first];
}  // Ссылка на первый элемент.
value_type& CircularBuffer::back() {
    if (empty()) throw std::out_of_range("Buffer empty");
    return buffer[end];
}  // Ссылка на последний элемент.
const value_type& CircularBuffer::front() const {
    if (empty()) throw std::out_of_range("Buffer empty");
    return buffer[first];
}
const value_type& CircularBuffer::back() const {
    if (empty()) throw std::out_of_range("Buffer empty");
    return buffer[end];
}

// Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
// переместится в начало аллоцированной памяти. Возвращает указатель
// на первый элемент.
value_type* CircularBuffer::linearize() {
    std::rotate(buffer, buffer + first, buffer + capacityBuff - 1);
    first = 0;
    end = capacityBuff - 1;
}
// Проверяет, является ли буфер линеаризованным.
bool CircularBuffer::is_linearized() const {
    if (first == 0) {
        return true;
    } else {
        return false;
    }
}
// Сдвигает буфер так, что по нулевому индексу окажется элемент
// с индексом new_begin.
void CircularBuffer::rotate(std::size_t new_begin) {
    std::rotate(buffer, buffer + new_begin - 1, buffer + capacityBuff - 1);
}

// Количество элементов, хранящихся в буфере.
std::size_t CircularBuffer::size() const {
    return sizeBuff;
}

bool CircularBuffer::empty() const {
    if (sizeBuff == 0) {
        return true;
    } else {
        return false;
    }
}
// true, если size() == capacity().
bool CircularBuffer::full() const {
    if (sizeBuff == capacityBuff) {
        return true;
    } else {
        return false;
    }
}
// Количество свободных ячеек в буфере.
std::size_t CircularBuffer::reserve() const {
    return capacityBuff - sizeBuff;
}
// ёмкость буфера
std::size_t CircularBuffer::capacity() const {
    return capacityBuff;
}

void CircularBuffer::set_capacity(std::size_t new_capacity) {
    linearize();
    value_type* newBuff = new value_type[new_capacity];
    std::copy_n(buffer, capacityBuff, newBuff);
    capacityBuff = new_capacity;
    delete[] buffer;
    buffer = newBuff;
}
// Изменяет размер буфера.
// В случае расширения, новые элементы заполняются элементом item.
void CircularBuffer::resize(std::size_t new_size, const value_type& item = value_type()) {
    if (new_size > capacityBuff) {
        set_capacity(new_size);
    } else {
        linearize();
    }
    if (new_size < sizeBuff) {
        end -= sizeBuff - new_size;
    } else if (new_size > sizeBuff) {
        for (std::size_t i = sizeBuff; i < new_size; ++i) {
            buffer[i] = item;
        }
    }
}
// Оператор присваивания.
CircularBuffer& CircularBuffer::operator=(const CircularBuffer& cb) {
    delete[] buffer;
    buffer = cb.buffer;
    cb.buffer = nullptr;
    sizeBuff = cb.sizeBuff;
    capacityBuff = cb.capacityBuff;
    first = cb.first;
    end = cb.end;
}
// Обменивает содержимое буфера с буфером cb.
void CircularBuffer::swap(CircularBuffer& cb) {
    std::swap(buffer, cb.buffer);
    std::swap(first, cb.first);
    std::swap(end, cb.end);
    std::swap(sizeBuff, cb.sizeBuff);
    std::swap(capacityBuff, cb.capacityBuff);
}

// Добавляет элемент в конец буфера.
// Если текущий размер буфера равен его ёмкости, то переписывается
// первый элемент буфера (т.е., буфер закольцован).
void CircularBuffer::push_back(const value_type& item = value_type()) {
    if (sizeBuff == capacityBuff) {
        end = first;
        first = (first + 1) % capacityBuff;
    } else {
        end++;
        sizeBuff++;
    }
    buffer[end] = item;
}
// Добавляет новый элемент перед первым элементом буфера.
// Аналогично push_back, может переписать последний элемент буфера.
void CircularBuffer::push_front(const value_type& item = value_type()) {
    if (first == 0) {
    }
}
// удаляет последний элемент буфера.
void CircularBuffer::pop_back() {}
// удаляет первый элемент буфера.
void CircularBuffer::pop_front() {}

// Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
void CircularBuffer::insert(std::size_t pos, const value_type& item = value_type()) {}
// Удаляет элементы из буфера в интервале [first, last).
void CircularBuffer::erase(std::size_t first, std::size_t last) {}
// Очищает буфер.
void CircularBuffer::clear() {
    sizeBuff = 0;
    first = 0;
    end = 0;
}
