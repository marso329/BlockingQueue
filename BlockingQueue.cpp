/*
 * BlockingQueue.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: msoderen
 */

#include "BlockingQueue.h"
#include "Structures.h"

template<typename T>
void BlockingQueue<T>::Put(const T& task){
    std::unique_lock<std::mutex> lock(mtx);
    while(queue_.size() == capacity_){
        full_.wait(lock );
    }
    assert(queue_.size() < capacity_);
    queue_.push(task);
    empty_.notify_all();
}

template<typename T>
T BlockingQueue<T>::Take(){
    std::unique_lock<std::mutex> lock(mtx);
    while(queue_.empty()){
        empty_.wait(lock );
    }
    assert(!queue_.empty());
    T front(queue_.front());
    queue_.pop();
    full_.notify_all();
    return front;
}

template<typename T>
size_t BlockingQueue<T>::Size(){
    std::lock_guard<std::mutex> lock(mtx);
    return queue_.size();
}

template<typename T>
T BlockingQueue<T>::Front(){
    std::unique_lock<std::mutex> lock(mtx);
    while(queue_.empty()){
        empty_.wait(lock );
    }
    assert(!queue_.empty());
    T front(queue_.front());
    return front;
}

template<typename T>
T BlockingQueue<T>::Back(){
    std::unique_lock<std::mutex> lock(mtx);
    while(queue_.empty()){
        empty_.wait(lock);
    }
    assert(!queue_.empty());
    T back(queue_.back());
    return back;
}

template<typename T>
bool BlockingQueue<T>::Empty(){
    std::unique_lock<std::mutex> lock(mtx);
    return queue_.empty();
}

template<typename T>
void BlockingQueue<T>::SetCapacity(const size_t capacity){
    capacity_ = (capacity > 0 ? capacity : MAX_CAPACITY);
}

template class BlockingQueue<QueueElement*>;
template class BlockingQueue<Unstable*>;
template class BlockingQueue<std::string>;
