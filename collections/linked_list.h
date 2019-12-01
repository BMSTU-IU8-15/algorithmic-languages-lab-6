#ifndef LAB_6_LINKED_LIST_H
#define LAB_6_LINKED_LIST_H

#include <cstddef>
#include <stdexcept>

using std::out_of_range;

namespace collections {

    template <typename T>
    class linked_list {

        /* ***************************************** Implementation details ***************************************** */

        class node {

            friend class linked_list;

            T value;
            node *previous = nullptr, *next = nullptr;

            explicit node(const T value): value(value) {}
        };

        size_t size_ = 0;
        node *first_ = nullptr, *last_ = nullptr;

    protected:

        node * node_at(const size_t index) const {
            const auto size = size_;

            if (index >= size) throw out_of_range("Index is out of range");

            if (index <= size << 1) { // lookup from head
                auto current = first_;
                size_t current_index = 0;
                while (current) {
                    if (current_index == index) return current;

                    current = current->next;
                    ++current_index;
                }
            } else { // lookup from tail
                auto current = last_;
                size_t current_index = size;
                while (current) {
                    --current_index;
                    if (current_index == index) return current;

                    current = current->next;
                }
            }
        }

        inline void push_first_node(node * const pushed) {
            if (size_ == 0) first_ = last_ = pushed;
            else {
                // double-link head
                pushed->next = first_;
                first_->previous = pushed;
                first_ = pushed;
            }

            ++size_;
        }

        inline void push_last_node(node * const pushed) {
            if (size_ == 0) first_ = last_ = pushed;
            else {
                pushed->previous = last_;
                last_->next = pushed;
                last_ = pushed;
            }

            ++size_;
        }

        inline void push_after_node(node * const previous, node * const pushed) {
            const auto old_next = previous->next;
            if (old_next) old_next->previous = pushed;
            else last_ = pushed;

            pushed->previous = previous;
            pushed->next = old_next;
            previous->next = pushed;

            ++size_;
        }

        inline void push_before_node(const node *next, const node *pushed) {
            const auto old_previous = next->previous;
            if (old_previous) old_previous->next = pushed;
            else first_ = pushed;

            pushed->next = next;
            pushed->previous = old_previous;
            next->previous = pushed;

            ++size_;
        }

        inline T pop_first_node() {
            switch (size_) {
                case 0: throw out_of_range("Attempt to pop a value from a non-empty collection");
                case 1: {
                    const auto popped = first_;
                    first_ = last_ = nullptr;

                    size_ = 0;
                    const auto value = popped->value;
                    delete popped;

                    return value;
                }
                default: {
                    const auto popped = first_;
                    first_ = popped->next;

                    --size_;
                    const auto value = popped->value;
                    delete popped;

                    return value;
                }
            }
        }

        inline void pop_first_node_no_return() {
            switch (size_) {
                case 0: throw out_of_range("Attempt to pop a value from a non-empty collection");
                case 1: {
                    const auto popped = first_;
                    first_ = last_ = nullptr;

                    size_ = 0;
                    delete popped;
                }
                default: {
                    const auto popped = first_;
                    first_ = popped->next;

                    --size_;
                    delete popped;
                }
            }
        }

        inline T pop_last_node() {
            switch (size_) {
                case 0: throw out_of_range("Attempt to pop a value from a non-empty collection");
                case 1: {
                    const auto popped = last_;
                    first_ = last_ = nullptr;

                    size_ = 0;
                    const auto value = popped->value;
                    delete popped;

                    return value;
                }
                default: {
                    const auto popped = last_;
                    last_ = popped->previous;

                    --size_;
                    const auto value = popped->value;
                    delete popped;

                    return value;
                }
            }
        }

        inline void pop_last_node_no_return() {
            switch (size_) {
                case 0: throw out_of_range("Attempt to pop a value from a non-empty collection");
                case 1: {
                    const auto popped = last_;
                    first_ = last_ = nullptr;

                    size_ = 0;
                    delete popped;
                }
                default: {
                    const auto popped = last_;
                    last_ = popped->previous;

                    --size_;
                    delete popped;
                }
            }
        }

        inline T pop_node(const node *popped) {
            const auto first = first_;
            if (popped == first) {
                if (popped == last_) first_ = last_ = nullptr; // pop the only one
                else { // simply pop the first
                    first_ = first->next;
                    first->next->previous = nullptr;
                }
            } else {
                auto last = last_;
                if (popped == last) { // simply pop the last
                    // popped != first
                    last = last->previous;
                    last->previous->next = nullptr;
                } else { // pop non-border
                    const auto previous = popped->previous, next = popped->next;
                    previous->next = next;
                    next->previous = previous;
                }
            }

            --size_;
            const auto value = popped->value;
            delete popped;
            return value;
        }

        inline void pop_node_no_return(const node * popped) {
            const auto first = first_;
            if (popped == first) {
                if (popped == last_) first_ = last_ = nullptr; // pop the only one
                else { // simply pop the first
                    first_ = first->next;
                    first->next->previous = nullptr;
                }
            } else {
                auto last = last_;
                if (popped == last) { // simply pop the last
                    // popped != first
                    last = last->previous;
                    last->previous->next = nullptr;
                } else { // pop non-border
                    const auto previous = popped->previous, next = popped->next;
                    previous->next = next;
                    next->previous = previous;
                }
            }


            --size_;
            delete popped;
        }

        /* ********************************************** API members ********************************************** */
    public:

        ~linked_list() {
            auto current = first_;
            while (current) {
                const auto next = current->next;
                delete current;
                current = next;
            }
        }

        [[nodiscard]] size_t size() const {
            return size_;
        }

        void push_first(const T value) {
            push_first_node(new node(value));
        }

        void push_last(const T value) {
            push_last_node(new node(value));
        }

        void push_after(const T value, const T& previous) {
            if (size_ == 0) return;

            auto current = first_;
            while (current) {
                if (current->value == previous) {
                    push_after_node(current, new node(value));

                    return;
                }

                current = current->next;
            }
        }

        void push_before(const T value, const T &next) {
            if (size_ == 0) return;

            auto current = first_;
            while (current) {
                if (current->value == next) {
                    push_before_node(current, new node(value));

                    return;
                }

                current = current->next;
            }
        }

        void push_at(const T value, const size_t index) {
            const auto size = size_;

            if (index == 0) push_first_node(new node(value));
            else if (index == size) push_last_node(new node(value));
            else if (index > size) throw out_of_range("Index is greater than size");
            else {
                if (index <= size << 1) {
                    // go from the head
                    auto current = first_;
                    size_t next_index = 1;
                    while (true) {
                        if (next_index == index) {
                            push_after_node(current, new node(value));
                            return;
                        }

                        current = current->next;
                        ++next_index;
                    }
                } else {
                    // go from the tail
                    auto current = last_;
                    size_t current_index = size - 1;
                    while (true) {
                        if (current_index == index) {
                            push_before_node(current, new node(value));
                            return;
                        }

                        current = current->previous;
                        --current_index;
                    }
                }
            }
        }

        T pop_first() {
            return pop_first_node();
        }

        T pop_last() {
            return pop_last_node();
        }

        void pop_one(const T &value) {
            if (size_ == 0) return;

            auto current = first_;
            while (current) {
                if (current->value == value) {
                    pop_node_no_return(current);
                    return;
                }

                current = current->next;
            }
        }

        T pop_at(const size_t index) {
            return pop_node(node_at(index));
        }

        bool contains(const T &value) const {
            auto current = first_;
            while (current) {
                if (current->value == value) return true;

                current = current->next;
            }

            return false;
        }

        size_t indexOf(const T &value) const {
            auto current = first_;
            size_t index = 0;
            while (current) {
                if (current->value == value) return index;

                current = current->next;
                ++index;
            }

            return -1;
        }

        inline T at(const size_t index) const {
            return node_at(index)->value;
        }

        template <typename CONSUMER>
        void for_each(const CONSUMER consumer) {
            if (size_ == 0) return;

            auto current = first_;
            size_t index = 0;
            while (current) {
                consumer(current->value);

                current = current->next;
                ++index;
            }
        }

        template <typename BI_CONSUMER>
        void for_each_indexed(const BI_CONSUMER consumer) {
            if (size_ == 0) return;

            auto current = first_;
            size_t index = 0;
            while (current) {
                consumer(index, current->value);

                current = current->next;
                ++index;
            }
        }

        /* *********************************************** Operators *********************************************** */

        inline T operator [](const size_t index) const {
            return at(index);
        }

        inline void operator +=(const T popped) {
            push_last(popped);
        }

        inline void operator -=(const T popped) {
            pop_one(popped);
        }

        inline explicit operator size_t() const {
            return size_;
        }
    };
}

#endif //LAB_6_LINKED_LIST_H
