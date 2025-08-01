#pragma once

namespace simulate_reverse_iterator {
    template<typename Iterator , typename Ref , typename Ptr>
    struct reverse_iterator {
        typedef reverse_iterator<Iterator , Ref , Ptr> self;

        reverse_iterator(Iterator iter) :iterator(iter) {}

        Ref operator*() {
            self temp(iterator);
            return *--temp;
        }

        Ptr operator->() {
            return &(operator*());
        }

        self& operator++() {
            return --iterator;
        }

        self& operator--() {
            return ++iterator;
        }

        bool operator==(const self& s) {
            return iterator == s.iterator;
        }

        bool operator!=(const self& s) {
            return iterator != s.iterator;
        }

        Iterator iterator;
    };
}