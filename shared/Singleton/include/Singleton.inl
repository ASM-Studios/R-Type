#pragma once

#include "Singleton.hpp"
#include <cassert>
#include <utility>

template <typename T>
Singleton<T>& Singleton<T>::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = std::unique_ptr<Singleton<T>>(new Singleton<T>());
    }
    return *_instance;
}

template <typename T>
template <typename ...Args>
Singleton<T>& Singleton<T>::wrap(Args... args) {
    Singleton<T>& instance = Singleton<T>::getInstance();
    instance._data.emplace(std::forward<Args>(args)...);
    return instance;
}

template <typename T>
Singleton<T>& Singleton<T>::wrap(T& data) {
    auto& instance = Singleton<T>::getInstance();
    instance._data = data;
    return instance;
}

template <typename T>
T& Singleton<T>::get() {
    if (this->_isSafe) {
        assert(this->_accessMutex.isLocked());
    }
    return this->_data.value();
}

template <typename T>
bool Singleton<T>::has_value() {
    if (this->_isSafe) {
        assert(this->_accessMutex.isLocked());
    }
    return this->_data.has_value();
}

template <typename T>
std::unique_ptr<Singleton<T>> Singleton<T>::_instance(nullptr);

template <typename T>
std::mutex Singleton<T>::_mutex;
