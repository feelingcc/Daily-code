#include <iostream>
#include <unordered_map>
#include <gtest/gtest.h>

class UnorderedMapEnvironmentTest : public testing::Test {
    public:
        static void SetUpTestCase() {
            std::cout << "在该测试套件第一个测试用例开始前执行" << std::endl;        
        }
        static void TearDownTestCase() {
            std::cout << "在该测试套件最后一个测试用例结束后执行" << std::endl;
        }
        virtual void SetUp() override {
            std::cout << "在该测试套件的每一个测试用例开始前执行" << std::endl;
            hash.insert({1, 1});
            hash.insert({2, 2});
            hash.insert({3, 3});
        };
        virtual void TearDown() override {
            std::cout << "在该测试套件的每一个测试用例结束后执行" << std::endl;
            hash.clear();
        }
    public:
        std::unordered_map<int,int> hash;
};

TEST_F(UnorderedMapEnvironmentTest , size) {   
    EXPECT_EQ(hash.size() , 3);
}

TEST_F(UnorderedMapEnvironmentTest , find) {
    EXPECT_EQ(hash.find(2)->second , 2);
}

int main(int argc , char* argv[]) {
    testing::InitGoogleTest(&argc , argv);
    return RUN_ALL_TESTS();
}

// std::unordered_map<int,int> hash;

// class GlobalEnvironment : public testing::Environment {
//     public:
//         virtual void SetUp() override {
//             std::cout << "在所有的测试之前执行" << std::endl;
//             hash.insert({1, 1});
//             hash.insert({2, 2});
//             hash.insert({3, 3});
//         };
//         virtual void TearDown() override {
//             std::cout << "在所有的测试之后执行" << std::endl;
//             hash.clear();
//         }
// };

// TEST(unordered_map_test , size) {
//     EXPECT_EQ(hash.size() , 3); // true
// }

// TEST(unordered_map_test , erase) {
//     hash.erase(1);
//     EXPECT_EQ(hash.size() , 3); // false
//     EXPECT_EQ(hash.size() , 2); // true
// }

// TEST(unordered_map_test , find) {
//     EXPECT_EQ(hash.find(1) , hash.end());   // true
// }

// int main(int argc , char* argv[]) {
//     testing::InitGoogleTest(&argc , argv);
//     testing::AddGlobalTestEnvironment(new GlobalEnvironment);
//     return RUN_ALL_TESTS();
// }

// TEST(unordered_map_test , insert) {
//     std::unordered_map<int,int> hash;
//     hash.insert({1, 1});
//     hash.insert({2, 2});
//     hash.insert({3, 3});
//     ASSERT_EQ(hash.size() , 3);
// }

// int main(int argc , char* argv[]) {

//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }