#pragma once

#include <forward_list>
#include <vector>

#include <iostream>

#include "Vector.h"
#include "List.h"

namespace pr {

template<typename K, typename V>
class HashMap {
public:
    class Entry {
    public:
        K key_;
        V value_;

        Entry(K key, V value)
        : key_(key), value_(value) {};
        Entry(const Entry& e) {
            key_ = e.key_;
            value_ = e.value_;
        }
        
        Entry& operator=(const Entry& e) {
            if (this == &e)
                return *this;
            
            key_ = e.key_;
            value_ = e.value_;

            return *this;
        }
    };
    typedef Vector<List<Entry>> buckets_t;
    
    class iterator {
    public:
        using value_type = Entry;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;

        iterator(buckets_t& buckets, size_t vit = 0)
        : buckets_(buckets), vit_(vit), lit_(buckets_[vit_].begin()) {
            next_bucket();
        }

        iterator(buckets_t& buckets, size_t vit, typename List<Entry>::iterator lit)
        : buckets_(buckets), vit_(vit), lit_(lit) {}
        iterator(const iterator& it)
        : buckets_(it.buckets_), vit_(it.vit_), lit_(it.lit_) {}

        iterator& operator++() {
            lit_++;
            if (!(lit_ != buckets_[vit_].end())) {
                vit_++;
                next_bucket();
            }
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this; 
            ++(*this); 
            return tmp; 
        }

        bool operator!=(const iterator& rhs) {
            return ((vit_ != rhs.vit_) 
                 || (lit_ != rhs.lit_));
        }
        bool operator==(const iterator& rhs) {
            return !(*this != rhs);
        }

        reference operator*() const {
            return *lit_;
        }
        pointer operator->() { 
            static value_type v; 
            v = *(*this); 
            return &v; 
        }

        void next_bucket() {
            while (buckets_[vit_].size() == 0 
                && vit_ < buckets_.size()) {
                vit_++;
            }
            lit_ = buckets_[vit_].begin();
        }

        buckets_t& buckets_;
        size_t vit_;
        typename List<Entry>::iterator lit_;
    };

private:
    buckets_t buckets_;

public:
    HashMap(size_t size=100) {
        // Pour éviter les ré-allocations
        buckets_.reserve(size);

        for (size_t i = 0; i < size; i++) {
            List<Entry> l;
            buckets_.push_back(l);
        }
    }

    V* get(const K& key) {
        // std::hash<K> est un foncteur (possède un opérateur '()')
        size_t h = std::hash<K>()(key) % buckets_.size();
        for (auto& e : buckets_[h]) {
            if (e.key_ == key)
                return &e.value_;
        }
        return nullptr;
    }

    bool put(const K& key, const V& value) {
        size_t h = std::hash<K>()(key) % buckets_.size();
        for (auto& e : buckets_[h]) {
            if (e.key_ == key) {
                e.value_ = value;
                return true;
            }
        }

        Entry e(key, value);
        buckets_[h].push_front(e);
        return false;
    }

    size_t size() const {
        size_t n = 0;
        for (auto& l : buckets_) {
            n += l.size();
        }
        return n;
    }

    std::vector<std::pair<std::string, int>> extract() {
        std::vector<std::pair<std::string, int>> out;
        for (auto& l : buckets_) {
            for (auto& e : l) {
                out.push_back(std::make_pair(e.key_, e.value_));
            }
        }
        return out;
    }

    iterator begin() { return iterator(buckets_); }
    iterator end() { 
        typename List<Entry>::iterator lend(nullptr);
        return iterator(buckets_, 100, lend); 
    }
};

}