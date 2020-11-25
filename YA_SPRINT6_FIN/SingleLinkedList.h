#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>
#include <algorithm>
#include <stack>

template <typename TType>
class SingleLinkedList {
    //NODE
    struct Node {
        //constructors
        Node() = default;
        Node(const TType& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        //defaults
        TType value;
        Node* next_node = nullptr;
    };

    //Iterator
    template <typename TValueType>
    class BasicIterator {
        //acces to data *friend
        friend class SingleLinkedList;
        //constructor
        explicit BasicIterator(Node* node)
            :node_(node) {
        }
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = TType;
        using difference_type = std::ptrdiff_t;
        using pointer = TValueType*;
        using reference = TValueType&;

        //constructor
        BasicIterator() = default;
        BasicIterator(const BasicIterator<TType>& other) noexcept
            :node_(other.node_) {
        }

        //logic opers
        BasicIterator& operator=(const BasicIterator& rhs) = default;
        [[nodiscard]] bool operator==(const BasicIterator<const TType>& rhs) const noexcept {
            return  this->node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const TType>& rhs) const noexcept {
            return !(*this == rhs);
        }
        [[nodiscard]] bool operator==(const BasicIterator<TType>& rhs) const noexcept {
            return  this->node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<TType>& rhs) const noexcept {
            return !(*this == rhs);
        }

        //maths
        BasicIterator& operator++() noexcept {
            this->node_ = this->node_->next_node;
            return *this;
        }
        BasicIterator operator++(int) noexcept {
            Node* old = node_;
            this->node_ = this->node_->next_node;
            return  BasicIterator(old);
        }
        [[nodiscard]] reference operator*() const noexcept {
            return this->node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            return &this->node_->value;
        }
    private:
        Node* node_ = nullptr;
    };

public:
    //Iter methods
    using value_type = TType;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<TType>;
    using ConstIterator = BasicIterator<const TType>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(this->head_.next_node);
    }
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(this->head_.next_node);
    }
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(this->head_.next_node);
    }
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&this->head_);
    }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&this->head_));
    }
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        Node* ptr = &this->head_;
        return ConstIterator(ptr);
    }

    //constructors
    SingleLinkedList() :head_(), size_(0) {};
    SingleLinkedList(std::initializer_list<TType> values) {
        SingleLinkedList tmp;
        this->fromContainer(tmp, values);
        this->swap(tmp);
    }
    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList tmp;
        this->fromContainer(tmp, other);
        this->swap(tmp);
    }
    ~SingleLinkedList() {
        Clear();
    }

    //std possibilities
    [[nodiscard]] size_t GetSize() const noexcept {
        return this->size_;
    }
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (this->size_ != 0) {
            return false;
        }
        return true;
    }
    void PushFront(const TType& value) {
        this->head_.next_node = new Node(value, this->head_.next_node);
        ++this->size_;
    }
    void Clear() {
        while (this->head_.next_node != nullptr) {
            Node* nxt_nd = this->head_.next_node->next_node;
            delete this->head_.next_node;
            this->head_.next_node = nxt_nd;
        }
        this->size_ = 0;
    }
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (*this == rhs) {
            return *this;
        }
        SingleLinkedList tmp_othrs(rhs);
        this->swap(tmp_othrs);
        return *this;
    }
    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_node, other.head_.next_node);
        std::swap(this->size_, other.size_);
    }
    Iterator InsertAfter(ConstIterator pos, const TType& value) {
        Node* beforeInserted = pos.node_;
        beforeInserted->next_node = new Node(value, beforeInserted->next_node);
        ++this->size_;
        return Iterator{ beforeInserted->next_node };
    }
    void PopFront() noexcept {
        if (this->size_ != 0) {
            Node* tmp_next = this->head_.next_node->next_node;
            delete this->head_.next_node;
            this->head_.next_node = tmp_next;
            this->size_--;
        }
    }
    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* beforeDeleted = pos.node_;
        Node* tmp_nxt = beforeDeleted->next_node->next_node;
        delete beforeDeleted->next_node;
        beforeDeleted->next_node = tmp_nxt;
        this->size_--;

        return Iterator{ beforeDeleted->next_node };
    }

private:
    Node head_;
    size_t size_ = 0;

    template<typename TContainer>
    static void fromContainer(SingleLinkedList& base_, TContainer& values) {
        std::stack<TType> inverter;
        for (const auto& value : values) {
            inverter.push(value);
        }
        while (!inverter.empty()) {
            base_.PushFront(inverter.top());
            inverter.pop();
        }
    }
};


//logics for class
template <typename TType>
void swap(SingleLinkedList<TType>& lhs, SingleLinkedList<TType>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename TType>
bool operator==(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    auto ptr_lhs = lhs.begin(), ptr_rhs = rhs.begin();
    for (; ptr_lhs != lhs.end(); ptr_lhs++, ptr_rhs++) {
        if (!(ptr_lhs == ptr_rhs)) {
            return false;
        }
    }
    return true;
}

template <typename TType>
bool operator!=(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    return !(lhs == rhs);
}

template <typename TType>
bool operator<(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    auto ptr_lhs = lhs.begin(), ptr_rhs = rhs.begin();
    for (; (ptr_lhs != lhs.end()) && (ptr_rhs != rhs.end()); ptr_lhs++, (void)ptr_rhs++) {
        if (*ptr_lhs < *ptr_rhs) {
            return true;
        }
        if (*ptr_rhs < *ptr_lhs) {
            return false;
        }
    }
    return (ptr_lhs == lhs.end()) && (ptr_rhs != rhs.end());
}

template <typename TType>
bool operator<=(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    return !(rhs < lhs);
}

template <typename TType>
bool operator>(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    return rhs < lhs;
}

template <typename TType>
bool operator>=(const SingleLinkedList<TType>& lhs, const SingleLinkedList<TType>& rhs) {
    return !(lhs < rhs);
}