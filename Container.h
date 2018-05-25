#pragma once

#include <iterator>

template<class T>
class Container {
    struct Node {
        T element_;
        Node *prev_;
        Node *next_;

        Node(T element, Node *prev, Node *next)
                : element_(element), prev_(prev), next_(next) {}
    };

    class iterator {
        Node *node_;

    public:
        explicit iterator(Node *node) : node_(node) {}

        iterator(const iterator &it) {
            node_ = it.node_;
        }

        bool operator!=(iterator const &other) {
            return node_ != other.node_;
        }

		T const &operator*() {
            return node_->element_;
        }

        iterator &operator++() {
            node_ = node_->next_;
            return *this;
        }
    };

    class const_iterator {
        const Node *node_;

    public:
        explicit const_iterator(const Node *node) : node_(node) {}

        const_iterator(const const_iterator &it) {
            node_ = it.node_;
        }

        bool operator!=(const const_iterator &other) {
            return node_ != other.node_;
        }

		T const &operator*() {
            return node_->element_;
        }

        const_iterator &operator++() {
            node_ = node_->next_;
            return *this;
        }
    };

    size_t size_;
    Node *head_;
    Node *tail_;

public:

    class empty_collection : public std::logic_error {
    public:
        empty_collection() : logic_error("Collection is empty") {}
    };

    Container() : size_(0), head_(nullptr), tail_(nullptr) {}

    Container(const Container &other) {
        head_ = new Node(other.head_->element_, nullptr, nullptr);

        auto prev = head_;
        for (auto i = ++other.cbegin(); i != other.cend(); ++i) {
            auto current = new Node(*i, prev, nullptr);
            prev->next_ = current;
            prev = current;
        }

        tail_ = prev;
        size_ = other.size_;
    }

    Container(Container &&other) : size_(other.size_), head_(other.head_), tail_(other.tail_) {
        other.size_ = 0;
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    Container &operator=(const Container &other) {
        if (this == &other) {
            return *this;
        }

        clear();

        head_ = new Node(other.head_->element_, nullptr, nullptr);

        auto prev = head_;
        for (auto i = ++other.cbegin(); i != other.cend(); ++i) {
            auto current = new Node(*i, prev, nullptr);
            prev->next_ = current;
            prev = current;
        }

        tail_ = prev;
        size_ = other.size_;

        return *this;
    }

    Container &operator=(Container &&other) {
        if (this == &other) {
            return *this;
        }

        clear();

        size_ = other.size_;
        head_ = other.head_;
        tail_ = other.tail_;

        other.size_ = 0;
        other.head_ = nullptr;
        other.tail_ = nullptr;

        return *this;
    }

    ~Container() {
        this->clear();
    }

    iterator begin() {
        return iterator(head_);
    }

    iterator end() {
        return iterator(nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(head_);
    }

    const_iterator cend() const {
        return const_iterator(nullptr);
    }

    void push_front(const T &element) {
        if (size_ == 0) {
            head_ = new Node(element, nullptr, nullptr);
            tail_ = head_;
        } else {
            auto node = new Node(element, nullptr, head_);
            head_->prev_ = node;
            head_ = node;
        }
        size_++;
    }

    void push_back(const T &element) {
        if (size_ == 0) {
            tail_ = new Node(element, nullptr, nullptr);
            head_ = tail_;
        } else {
            auto node = new Node(element, tail_, nullptr);
            tail_->next_ = node;
            tail_ = node;
        }
        size_++;
    }

    void pop_front() {
        if (size_ == 0) {
            throw empty_collection();
        }

        if (size_ == 1) {
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            auto deleteLater = head_;
            head_->next_->prev_ = nullptr;
            head_ = head_->next_;
            delete deleteLater;
        }
        size_--;
    }

    void pop_back() {
        if (size_ == 0) {
            throw empty_collection();
        }

        if (size_ == 1) {
            delete tail_;
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            auto deleteLater = tail_;
            tail_->prev_->next_ = nullptr;
            tail_ = tail_->prev_;
            delete deleteLater;
        }
        size_--;
    }

    T front() const {
        return head_->element_;
    }

    T back() const {
        return tail_->element_;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        if (size_ == 0) { return; }

        auto i = head_;
        while (i->next_ != nullptr) {
            i = i->next_;
            delete i->prev_;
        }
        delete i;
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr;
    }
};
