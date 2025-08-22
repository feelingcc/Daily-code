#include "RBT.hpp"

namespace simulate_set {
    template <typename Key>
    class set {
        struct keyOfKey {
            Key& operator()(const Key& key) {
                return key;
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
        };
        private:
            RBT::RBTree<Key , const Key , keyOfKey> rb;
    };
}