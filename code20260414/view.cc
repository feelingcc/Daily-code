#include <odb/database.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/database.hxx>

#include "student.h"
#include "student-odb.hxx"

const std::string HOST = "127.0.0.1";
const std::string USER = "root";
const std::string PASS = "1";
const std::string DBNAME = "test";
const unsigned int PORT = 3306;
const std::string CSET = "utf8";

void select(std::unique_ptr<odb::mysql::database>& handler) {
    try {
        //1. 通过句柄构造事务对象； 
        odb::transaction tx(handler->begin());
        //2. 通过事务对象构造database对象；
        odb::database& db= tx.database();
        //3. 数据操作
        using Query = odb::query<ClassesStudent>;
        using Result = odb::result<ClassesStudent>;
        Result result(db.query<ClassesStudent>(Query::Classes::name == "二年级一班"));
        for(auto it = result.begin(); it != result.end(); ++it) {
            if(it->student) 
                std::cout << "班级:" << it->classes->name() << "\t";
            if(it->classes)
                std::cout << "姓名:" << it->student->name() << std::endl;
        }
        //4. 提交事务
        tx.commit();
    }catch (odb::exception &e) {
        std::cout << "odb error: " << e.what() << std::endl;
    }
}



int main() {

    // 1. 构造连接池
    std::unique_ptr<odb::mysql::connection_factory> pool(new odb::mysql::connection_pool_factory(5));
    // 2. 构造数据库操作句柄
    std::unique_ptr<odb::mysql::database> handler = std::make_unique<odb::mysql::database>(USER, PASS, DBNAME, HOST, PORT, nullptr, CSET, 0, std::move(pool));

    select(handler);

    return 0;
}