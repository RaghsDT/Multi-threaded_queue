#include <mutex>
#include <condition_variable>
#include <chrono>
#include <stdexcept>
#include <thread>

/**
 * @brief A thread-safe queue with a fixed capacity.
 *
 * Implements blocking and timeout-based pop operations.
 * If full, drops the oldest element before inserting a new one.
 *
 * @tparam T The type of elements stored in the queue.
 */
template <typename T>
class Queue {
public:
    /**
     * @brief Constructs a queue with the given capacity.
     * @param size Maximum number of elements the queue can hold.
     */
    explicit Queue(int size);

    /**
     * @brief Pushes an element into the queue.
     * If the queue is full, the oldest element is dropped.
     * @param element The element to push.
     */
    void Push(T element);

    /**
     * @brief Pops an element from the queue.
     * Blocks indefinitely if the queue is empty.
     * @return The front element of the queue.
     */
    T Pop();

    /**
     * @brief Pops an element with a timeout.
     * If the queue is empty and timeout expires, throws std::runtime_error.
     * @param milliseconds Timeout duration.
     * @return The front element of the queue.
     * @throws std::runtime_error if timeout expires without an element.
     */
    T PopWithTimeout(int milliseconds);

    /**
     * @brief Returns the current number of stored elements.
     * @return Number of elements in the queue.
     */
    int Count();

    /**
     * @brief Returns the maximum capacity of the queue.
     * @return The queue's maximum size.
     */
    int Size();

private:
    int capacity;             ///< Maximum number of elements in the queue.
    int count;                ///< Current number of elements.
    int front, rear;          ///< Front and rear index for the queue.
    T* buffer;                ///< Dynamic array storing queue elements.

    std::mutex mtx;           ///< Mutex for thread safety.
    std::condition_variable cv; ///< Condition variable for synchronization.

    /**
     * @brief Moves front index forward circularly.
     */
    void DropOldest();
};
