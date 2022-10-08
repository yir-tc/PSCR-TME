#pragma once

namespace pr {

template<typename T>
class List {
    class Chainon {
    public:
        Chainon* next_;
        T data_;

        Chainon(const T& data, Chainon *next=nullptr) 
        : next_(next), data_(data) {}
        // Pas d'effet à delete nullptr
        ~Chainon() { delete next_; }
    };
    Chainon *tete_;

public:
    class iterator {
    public:
        //ERREUR: les attributs de la classe parent n'appartiennent pas
        //necessairement à une classe inclue
        //iterator() : current_node_(tete_) {}
        iterator(Chainon *node) : current_node_(node) {}
        iterator(const iterator& it)
        : current_node_(it.current_node_) {}
        iterator& operator=(const iterator&) = default;

        T& operator*() const {
            return current_node_->data_;
        }

        iterator& operator++() {
            if (current_node_ != nullptr)
                current_node_ = current_node_->next_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator!=(const iterator& rhs) {
            return (current_node_ != rhs.current_node_);
        }
    private:
        Chainon *current_node_;     
    };

    List() : tete_(nullptr) {}
    ~List() {
        delete tete_;
    }

    T& operator[](size_t index) {
        Chainon *curr = tete_;
        for (size_t i = 0; i < index; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    size_t size() const {
        size_t i = 0;
        for (Chainon *curr = tete_; 
            curr != nullptr; 
            curr = curr->next_, i++) {}
        return i;
    }

    void push_front(const T& val) {
        tete_ = new Chainon(val, tete_);
    }

    bool empty() const {
        return (tete_ == nullptr);
    }

    iterator begin() { return iterator(tete_); }
    iterator end() { return iterator(nullptr); }
};

}