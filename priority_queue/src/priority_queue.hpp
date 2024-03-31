#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    private:
        struct node {
            T *data;
            node *left, *right;
            int npl;

            node() {
                data = nullptr;
                left = nullptr;
                right = nullptr;
                npl = 0;
            }

            ~node() {
                delete data;
                data = nullptr;
                left = nullptr;
                right = nullptr;
                npl = 0;
            }

            bool operator<(const node &other) {
                return !Compare()(*data , *(other.data));
            }
        };

        node *root;
        size_t size1;

        node *merge(node *l, node *r) {
            if (l == nullptr) return r;
            if (r == nullptr) return l;
            node* tmp;
            if (*l < *r) tmp= merge1(l, r);
            else tmp= merge1(r, l);
            //delete r;
            return tmp;
        }

        void swapChildren(node *t) {
            node *tmp;
            tmp = t->left;
            t->left = t->right;
            t->right = tmp;
        }

        node *merge1(node *l, node *r) {
            if (l->left == nullptr) { l->left = r; }
            else {
                l->right = merge(l->right, r);
                if (l->left->npl < l->right->npl)
                    swapChildren(l);
                l->npl = l->right->npl + 1;
            }
            return l;
        }

        node *build(node *t, node *other) {
            if (other == nullptr) {
                return nullptr;
            }
            t = new node;
            if(t->data== nullptr) t->data=new T(*(other->data));
            else *(t->data) = *(other->data);
            t->npl = other->npl;
            t->left = build(t->left, other->left);
            t->right = build(t->right, other->right);
            return t;
        }

        /*void Copy(BiTree t,BiTree &newt){
            if(t==NULL){
                newt = NULL;
                return;
            }
            else{
                newt = new BiTNode;
                newt->data = t->data;
                Copy(t->lchild,newt->lchild);
                Copy(t->rchild,newt->rchild);
            }
        }*/

        void clear(node *t) {
            if (t == nullptr) return;
            clear(t->left);
            clear(t->right);
            delete t;
            t = nullptr;
        }

    public:
        /**
         * TODO constructors
         */
        priority_queue() {
            root = nullptr;
            size1 = 0;
        }

        priority_queue(const priority_queue &other) {
            root = build(root, other.root);
            size1 = other.size1;
        }

        /**
         * TODO deconstructor
         */
        ~priority_queue() {
            clear(root);
            root = nullptr;
            size1 = 0;
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            if (this==&other) return *this;
            clear(root);
            root= build(root,other.root);
            size1=other.size1;
            return *this;
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if (empty()) { throw container_is_empty(); }
            return *(root->data);
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            //auto p1 = new node(e);
            auto p1 = new node;
            p1->data = new T(e);
            try {
                root = merge(p1, root);
            }
            catch (...){
                delete p1;
                throw runtime_error();
            }
            size1++;
        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (empty()) throw container_is_empty();
            auto tmp = root;
            root = merge(root->left, root->right);
            delete tmp;
            size1--;
        }

        /**
         * return the number of the elements.
         */
        size_t size() const {
            return size1;
        }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const {
            return root == nullptr;
        }

        /**
         * merge two priority_queues with at most O(logn) complexity.
         * clear the other priority_queue.
         */
        void merge(priority_queue &other) {
            if (this == &other) return;
            root = merge(root, other.root);
            //delete other.root;
            other.root = nullptr;
            size1 += other.size1;
        }
    };

}

#endif