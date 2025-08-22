#include "RBT.hpp"

namespace simulate_map {
    template <typename Key , typename Value>
    class map {
        struct keyOfValue {
            Key& operator()(const std::pair<Key , Value>& data) {
                return data.first;
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
        };
        private:
            RBT::RBTree<Key , std::pair<const Key , Value> , keyOfValue> rb;
    };
}