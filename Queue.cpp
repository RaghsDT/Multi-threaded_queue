#include "Queue.h"

template <typename T>
Queue<T>::Queue(int size) : capacity(size), count(0), front(0), rear(0) {
    buffer = new T[capacity];  // Allocate memory dynamically
}

template <typename T>
void Queue<T>::Push(T element) {
    std::unique_lock<std::mutex> lock(mtx);

    if (count == capacity) {
        DropOldest();  // Remove the oldest element if full
    }

    buffer[rear] = element;
    rear = (rear + 1) % capacity;
    count++;

    cv.notify_one();  // Notify waiting threads
}

template <typename T>
T Queue<T>::Pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return count > 0; });  // Wait until an element is available

    T value = buffer[front];
    front = (front + 1) % capacity;
    count--;

    return value;
}

template <typename T>
T Queue<T>::PopWithTimeout(int milliseconds) {
    std::unique_lock<std::mutex> lock(mtx);

    if (!cv.wait_for(lock, std::chrono::milliseconds(milliseconds), [this] { return count > 0; })) {
        throw std::runtime_error("PopWithTimeout: Timed out waiting for an element.");
    }

    T value = buffer[front];
    front = (front + 1) % capacity;
    count--;

    return value;
}

template <typename T>
int Queue<T>::Count() {
    std::lock_guard<std::mutex> lock(mtx);
    return count;
}

template <typename T>
int Queue<T>::Size() {
    return capacity;
}

template <typename T>
void Queue<T>::DropOldest() {
    front = (front + 1) % capacity;
    count--;
}
