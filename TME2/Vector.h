#pragma once

namespace pr {

template<typename T>
class Vector {
public:
    Vector(int size=10) : alloc_size_(size), size_(0) {
        tab_ = new T[alloc_size_];
    }
    Vector(const Vector& v) : alloc_size_(v.alloc_size_), size_(v.size_) {
        tab_ = new T[alloc_size_];
        for (size_t i = 0; i < size_; i++) {
            tab_[i] = v.tab_[i];
        }
    }
    Vector& operator=(const Vector& v) {
        if (this == &v)
            return *this;
        
        delete [] tab_;

        alloc_size_ = v.alloc_size_;
        size_ = v.size_;
        tab_ = new T[alloc_size_];

        for (size_t i = 0; i < v.size_; i++) {
            tab_[i] = v.tab_[i];
        }

        return *this;
    }

    ~Vector() {
        delete [] tab_;
    };
    
    T& operator[](size_t index) {
        return tab_[index];
    };
    // Prototype différent: le this (argument implicite) est typé différement:
    // il est const ici
    const T& operator[](size_t index) const {
        return tab_[index];
    };

    void push_back(const T& val) {
        if (size_ + 1 >= alloc_size_)
           grow(alloc_size_);
        tab_[size_++] = val;
    }
    
    void push_front(const T& val) {
        if (size_ + 1 >= alloc_size_)
            grow(alloc_size_);
        for (size_t i = size_; i > 0; i--) {
            tab_[i] = tab_[i - 1];
        }
        tab_[0] = val;
        size_++;
    }

    void reserve(size_t n) {
        if (alloc_size_ < n) {
            grow(n - alloc_size_);
        }
    }

    size_t size() const { return size_; }
    bool empty() const { return (size_ == 0); }

    // Pas besoin d'un itérateur ici, puisque
    // on manipule directement des pointeurs
    T* begin() const { return tab_; }
    T* end() const { return tab_ + size_; }

private:
    void grow(size_t n) {
        alloc_size_ += n;
        T *tmp = new T[alloc_size_];
        for (size_t i = 0; i < size_; i++)
            tmp[i] = tab_[i];
        delete [] tab_;
        tab_ = tmp;
    }

    size_t alloc_size_;
    size_t size_;
    T* tab_;
};

}