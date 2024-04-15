/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
        //friend class iterator;
        //friend class const_iterator;
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;

        pair<const Key, T> operator<(const pair<const Key, T> &other) {
            return Compare()(this->first, other.first);
        }

        pair<const Key, T> operator==(const pair<const Key, T> &other) {
            return !(Compare()(this->first, other.first) || Compare()(other.first, this->first));
        }

        pair<const Key, T> operator>(const pair<const Key, T> &other) {
            return Compare()(other.first, this->first);
        }

        Key operator<(const Key &other) {
            return Compare()(*this, other);
        }

        Key operator==(const Key &other) {
            return !(Compare()(*this, other) || Compare()(other, *this));
        }

        Key operator>(const Key &other) {
            return Compare()(other, *this);
        }

        class const_iterator;

        class iterator;

    private:
        struct node {
            value_type **data;
            node *left, *right, *parent;
            int height;

            node() {
                data = new value_type *;
                left = nullptr;
                right = nullptr;
                parent = nullptr;
                height = 0;
            }

            node(const value_type &t) {
                data = new value_type *;
                *data = new value_type(t);
                left = nullptr;
                right = nullptr;
                parent = nullptr;
                height = 0;
            }

            ~node() {
                delete *data;
                delete data;
                left = nullptr;
                right = nullptr;
                parent = nullptr;
                height = 0;
            }
        };

        //static
        node *root;
        size_t size1;

    private:
        /*node *beroot() {
            return root;
        }*/

        int height(node *t) {
            if (t == nullptr) return 0;
            return t->height;
        }

        void clear(node *&t) {
            if (t == nullptr) return;
            clear(t->left);
            clear(t->right);
            delete t;
            t = nullptr;
        }

        void removes(const Key &x) {
            removes(x, root);
        }

        bool removes(const Key &x, node *&t) {
            if (t == nullptr) return true;
            if (!(Compare()(x, (*t->data)->first) || Compare()((*t->data)->first, x))) {
                if (t->left == nullptr || t->right == nullptr) {
                    node *old = t;;
                    t = (t->left != nullptr) ? t->left : t->right;
                    if (t != nullptr) t->parent = old->parent;
                    delete old;
                    return false;
                } else {
                    node *tmp = t->right;
                    while (tmp->left != nullptr) tmp = tmp->left;
                    auto linshi = t->data;
                    (t->data) = (tmp->data);
                    tmp->data = linshi;
                    if (removes((*linshi)->first, t->right)) return true;
                    return adjust(t, 1);
                }
            }
            if (Compare()(x, (*t->data)->first)) {
                if (removes(x, t->left)) return true;
                return adjust(t, 0);
            } else {
                if (removes(x, t->right)) return true;
                return adjust(t, 1);
            }
        }

        bool adjust(node *&t, int subTree) {
            if (subTree) {
                if (height(t->left) - height(t->right) == 1) return true;
                if (height(t->left) == height(t->right)) {
                    --t->height;
                    return false;
                }
                if (height(t->left->right) > height(t->left->left)) {
                    LR(t);
                    return false;
                }
                LL(t);
                if (height(t->right) == height(t->left)) return false;
                return true;
            } else {
                if (height(t->right) - height(t->left) == 1) return true;
                if (height(t->left) == height(t->right)) {
                    --t->height;
                    return false;
                }
                if (height(t->right->left) > height(t->right->right)) {
                    RL(t);
                    return false;
                }
                RR(t);
                if (height(t->right) == height(t->left)) return false;
                return true;
            }
        }

        inline int max(int t1, int t2) {
            if (t1 < t2) return t2;
            return t1;
        }

        node *build(node *other) {
            if (other == nullptr) return nullptr;
            node *tmp = new node(**(other->data));
            //*(tmp->data) = new value_type;
            //**(tmp->data) = ;
            tmp->height = other->height;
            tmp->left = build(other->left);
            tmp->right = build(other->right);
            if (tmp->left)tmp->left->parent = tmp;
            if (tmp->right)tmp->right->parent = tmp;
            return tmp;
        }

        node *search(node *t, const Key &key) {
            if (t == nullptr)
                return nullptr;//throw index_out_of_bound();
            if (!(Compare()((*(t->data))->first, key) || Compare()(key, (*(t->data))->first))) return t;
            else if (Compare()(key, (*(t->data))->first))
                return search(t->left, key);
            else
                return search(t->right, key);
        }

        void LL(node *&t) {
            node *lson = t->left;
            if (lson->right) lson->right->parent = t;//处理lson的右儿子
            t->left = lson->right;
            lson->parent = t->parent;
            lson->right = t;
            t->parent = lson;
            t->height = max(height(t->left), height(t->right)) + 1;
            lson->height = max(height(lson->right), height(t->right)) + 1;
            t = lson;
        }

        void RR(node *&t) {
            node *rson = t->right;
            if (rson->left) rson->left->parent = t;
            t->right = rson->left;
            rson->parent = t->parent;
            rson->left = t;
            t->parent = rson;
            t->height = max(height(t->left), height(t->right)) + 1;
            rson->height = max(height(rson->right), height(t->right)) + 1;
            t = rson;
        }

        void LR(node *&t) {
            RR(t->left);
            LL(t);
        }

        void RL(node *&t) {
            LL(t->right);
            RR(t);
        }

        pair<iterator, bool> insert(const value_type &x, node *&t) {
            if (t == nullptr) {
                t = new node(x);
                //**(t->data) = ;
                size1++;
                t->height = max(height(t->left), height(t->right)) + 1;
                return pair<iterator, bool>{iterator(t, this), true};
            } else if (Compare()(x.first, (*t->data)->first)) {
                pair<iterator, bool> tmp = insert(x, t->left);
                t->left->parent = t;
                if (height(t->left) - height(t->right) == 2) {
                    if (Compare()(x.first, (*t->left->data)->first)) LL(t);
                    else LR(t);
                }
                t->height = max(height(t->left), height(t->right)) + 1;
                return tmp;
            } else if (Compare()((*t->data)->first, x.first)) {
                pair<iterator, bool> tmp = insert(x, t->right);
                t->right->parent = t;
                if (height(t->right) - height(t->left) == 2) {
                    if (Compare()((*t->right->data)->first, x.first)) RR(t);
                    else RL(t);
                }
                t->height = max(height(t->left), height(t->right)) + 1;
                return tmp;
            } else {
                t->height = max(height(t->left), height(t->right)) + 1;
                return pair<iterator, bool>{iterator(t, this), false};
            }
        }

    public:
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;

        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */

        public:
            node *ptr;
            map *p;

            iterator() {
                ptr = nullptr;
                p = nullptr;
            }

            iterator(node *t, map *tp) {
                ptr = t;
                p = tp;
            }

            iterator(const iterator &other) {
                ptr = other.ptr;
                p = other.p;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                // 有右子节点就变成右子节点；没有右子节点：如果是父节点的左子节点就变成父节点，
                // 如果是父节点的右子节点就一直向上，直到自己变成左子节点，变成父节点。
                iterator tmp(*this);
                /*if (ptr->right) {
                    ptr = ptr->right;
                    return tmp;
                }
                while (ptr != ptr->parent->left) {
                    ptr = ptr->parent;
                }
                ptr = ptr->parent;*/
                ++*this;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                if (ptr->right) {
                    ptr = ptr->right;
                    while (ptr->left) ptr = ptr->left;
                    return *this;
                }
                while (ptr != ptr->parent->left) {
                    ptr = ptr->parent;
                }
                ptr = ptr->parent;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(*this);
                --*this;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                if (ptr == nullptr) {
                    ptr = p->root;
                    while (ptr->right) ptr = ptr->right;
                    return *this;
                }
                if (ptr->left) {
                    ptr = ptr->left;
                    while (ptr->right) ptr = ptr->right;
                    return *this;
                }
                while (ptr != ptr->parent->right) {
                    ptr = ptr->parent;
                }
                ptr = ptr->parent;
                return *this;
            }

            value_type &operator*() const {
                return (ptr->data);//?
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator==(const const_iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return ptr != rhs.ptr;
            }

            bool operator!=(const const_iterator &rhs) const {
                return ptr != rhs.ptr;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return *(ptr->data);//?
            }
        };

        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
        public:
            const node *ptr;
            const map *p;

            const_iterator() {
                ptr = nullptr;
                p = nullptr;
            }

            const_iterator(const node *t, const map *tp) : ptr(t), p(tp) {}

            const_iterator(const const_iterator &other) : ptr(other.ptr), p(other.p) {}

            const_iterator(const iterator &other) {
                ptr = other.ptr;
                p = other.p;
            }

            const_iterator operator++(int) {
                const_iterator tmp(*this);
                ++*this;
                return tmp;
            }

            const_iterator &operator++() {
                if (ptr->right) {
                    ptr = ptr->right;
                    while (ptr->left) ptr = ptr->left;
                    return *this;
                }
                while (ptr->parent != nullptr && ptr != ptr->parent->left) {
                    ptr = ptr->parent;
                }
                ptr = ptr->parent;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp(*this);
                --*this;
                return tmp;
            }

            const_iterator &operator--() {
                if (ptr == nullptr) {
                    ptr = p->root;
                    while (ptr->right) ptr = ptr->right;
                    return *this;
                }
                if (ptr->left) {
                    ptr = ptr->left;
                    while (ptr->right) ptr = ptr->right;
                    return *this;
                }
                while (ptr != ptr->parent->right) {
                    ptr = ptr->parent;
                }
                ptr = ptr->parent;
                return *this;
            }

            value_type &operator*() const {
                return (ptr->data);//?
            }

            bool operator==(const iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator==(const const_iterator &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(const iterator &rhs) const {
                return ptr != rhs.ptr;
            }

            bool operator!=(const const_iterator &rhs) const {
                return ptr != rhs.ptr;
            }

            value_type *operator->() const noexcept {
                return *(ptr->data);//?
            }
        };

        /**
         * TODO two constructors
         */
        map() {
            root = nullptr;
            size1 = 0;
        }

        map(const map &other) {
            if (root == other.root) return;
            root = nullptr;
            size1 = 0;
            root = build(other.root);
            size1 = other.size1;
        }

        /**
         * TODO assignment operator
         */
        map &operator=(const map &other) {
            if (&other == this) return *this;
            if (!empty()) clear();
            root = build(other.root);
            size1 = other.size1;
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~map() {
            clear();
            root = nullptr;
            size1 = 0;
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key) {
            node* t=search(root, key);
            if(t== nullptr) throw index_out_of_bound();
            return (*(t->data))->second;
        }

        const T &at(const Key &key) const {
            node* t=search(root, key);
            if(t== nullptr) throw index_out_of_bound();
            return (*(t->data))->second;
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            auto x=insert({key, T()});
            node *t = x.first.ptr;
            return (*t->data)->second;
            /*try { return at(key); }
            catch (...) {
                insert({key, T()});
                return at(key);
            }*/
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            return at(key);
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            node *ptr = root;
            if (!empty()) while (ptr->left) ptr = ptr->left;
            iterator it(ptr, this);
            return it;
        }

        const_iterator cbegin() const {
            node *ptr = root;
            if (!empty()) while (ptr->left) ptr = ptr->left;
            const_iterator it(ptr, this);
            return it;
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {        //!!!!!!!!!!!
            iterator it(nullptr, this);
            return it;
        }

        const_iterator cend() const {
            const_iterator it(nullptr, this);
            return it;
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return root == nullptr;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return size1;
        }

        /**
         * clears the contents
         */
        void clear() {
            clear(root);
            root = nullptr;
            size1 = 0;
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            //auto tmp=tmp;
            //size1++;
            return insert(value, root);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            removes((*pos.ptr->data)->first, root);
            size1--;
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            node *t = root;
            while (t != nullptr) {
                if (!(Compare()((*t->data)->first, key) || Compare()(key, (*t->data)->first))) break;
                if (Compare()(key, (*t->data)->first)) t = t->left;
                else t = t->right;
            }
            if (t != nullptr) return 1;
            return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            node *t = root;
            while (t != nullptr) {
                if (!(Compare()((*t->data)->first, key) || Compare()(key, (*t->data)->first)))
                    break;
                if (Compare()(key, (*t->data)->first))
                    t = t->left;
                else
                    t = t->right;
            }
            iterator it(t, this);
            return it;
        }

        const_iterator find(const Key &key) const {}
    };

}

#endif
