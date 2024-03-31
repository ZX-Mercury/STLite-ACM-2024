#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;

        class iterator {
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            T **current;

            /*iterator operator=(const iterator &rhs) {
                current = rhs.current;
                return *this;
            }*/

            iterator operator+(const int &n) const {
                iterator tmp = *this;
                tmp.current = current + n;
                return tmp;
            }

            iterator operator-(const int &n) const {
                iterator tmp = *this;
                tmp.current = current - n;
                return tmp;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (false) { throw invalid_iterator; }
                return rhs.current - current;
            }

            iterator &operator+=(const int &n) {
                current += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                current -= n;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                current++;
                return tmp;
            }

            iterator &operator++() {
                current++;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                current--;
                return tmp;
            }

            iterator &operator--() {
                current--;
                return *this;
            }

            T &operator*() const {
                return **current;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return current == rhs.current;
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return current != rhs.current;
            }

            bool operator!=(const const_iterator &rhs) const {
                return current != rhs.current;
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

        private:
        public:
            T **current;

            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            const_iterator operator+(const int &n) const {
                const_iterator tmp = *this;
                tmp.current = current + n;
                return tmp;
            }

            const_iterator operator-(const int &n) const {
                const_iterator tmp = *this;
                tmp.current = current - n;
                return tmp;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                if (false) { throw invalid_iterator; }
                return rhs.current - current;
            }

            const_iterator &operator+=(const int &n) {
                current += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                current -= n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                current++;
                return tmp;
            }

            const_iterator &operator++() {
                current++;
                return *this;
            }

            const_iterator operator--(int) {
                iterator tmp = *this;
                current++;
                return tmp;
            }

            const_iterator &operator--() {
                current--;
                return *this;
            }

            T &operator*() const {
                return **current;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return current == rhs.current;
            }

            bool operator==(const const_iterator &rhs) const {
                return current == rhs.current;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return current != rhs.current;
            }

            bool operator!=(const const_iterator &rhs) const {
                return current != rhs.current;
            }

        };

        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        vector() : data(nullptr), maxSiz(10) {
            curLen = 0;
            data = new T *[maxSiz];

        }

        vector(const vector &other) {
            curLen = 0;
            maxSiz = other.maxSiz;
            data = new T *[maxSiz];
            for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
                push_back(*it);
            }
        }

        /**
         * TODO Destructor
         */
        ~vector() {
            clear();
            delete[] data;
            data = nullptr;
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            vector<T> tmp = vector<T>(other);
            auto f1 = data;
            data = tmp.data;
            tmp.data = f1;

            auto f2 = curLen;
            curLen = tmp.curLen;
            tmp.curLen = f2;

            auto f3 = maxSiz;
            maxSiz = tmp.maxSiz;
            tmp.maxSiz = f3;

            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos) {
            if (pos >= size() || pos < 0) { throw index_out_of_bound; }
            return *(begin() + pos);
        }

        const T &at(const size_t &pos) const {
            return *(cbegin() + pos);
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos) {
            return at(pos);
        }

        const T &operator[](const size_t &pos) const {
            return at(pos);
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const {
            if (empty()) { throw container_is_empty; }
            return *(cbegin());
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const {
            if (empty()) {}
            return *(cend() - 1);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator tmp;
            tmp.current = data;
            return tmp;
        }

        const_iterator cbegin() const {
            const_iterator tmp;
            tmp.current = data;
            return tmp;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator tmp;
            tmp.current = data + curLen;
            return tmp;
        }

        const_iterator cend() const {
            const_iterator tmp;
            tmp.current = data + curLen;
            return tmp;
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return curLen == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            size_t tmp;
            tmp = curLen;
            return tmp;
        }

        /**
         * clears the contents
         */
        void clear() {
            while (!empty()) {
                pop_back();
            }
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            iterator it, it2;
            it = end();
            it2 = end() - 1;
            while (it != pos) {
                *(it.current) = *(it2.current);
                it--;
                it2--;
            }
            *(it.current) = new T(value);
            curLen++;
            if (maxSiz == curLen + 1) expandSpace();
            return it;
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > size()) throw index_out_of_bound();
            return insert(begin() + ind, value);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            delete *(pos.current);
            iterator it, it2;
            it = pos;
            it2 = pos + 1;
            while (it != end() - 1) {
                *(it.current) = *(it2.current);
                it++;
                it2++;
            }
            it2.current = nullptr;
            curLen--;
            if (maxSiz == curLen * 4) reduceSpace();
            return pos;
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind >= size()) throw index_out_of_bound;
            return erase(begin() + ind);
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            insert(end(), value);
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (empty()) { throw container_is_empty(); }
            erase(end() - 1);
        }

    private:
        T **data;
        int curLen;
        int maxSiz;

        void expandSpace() {
            T **tmp = new T *[maxSiz * 2];
            for (auto it = data; it != end().current; it++) {
                tmp[it - data] = *it;
            }
            maxSiz *= 2;
            delete[] data;
            data = tmp;
        };

        void reduceSpace() {
            T **tmp = new T *[maxSiz / 2];
            for (auto it = data; it != end().current; it++) {
                tmp[it - data] = *it;
            }
            maxSiz /= 2;
            delete[] data;
            data = tmp;
        }
    }
}

#endif