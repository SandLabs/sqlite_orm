#include <sqlite_orm/sqlite_orm.h>
#include <catch2/catch.hpp>

using namespace sqlite_orm;

TEST_CASE("In"){
    {
        struct User {
            int id;
        };
        
        auto storage = make_storage("",
                                    make_table("users",
                                               make_column("id", &User::id, primary_key())));
        storage.sync_schema();
        storage.replace(User{ 1 });
        storage.replace(User{ 2 });
        storage.replace(User{ 3 });
        
        {
            auto rows = storage.get_all<User>(where(in(&User::id, {1, 2, 3})));
            REQUIRE(rows.size() == 3);
        }
        {
            std::vector<int> inArgument;
            inArgument.push_back(1);
            inArgument.push_back(2);
            inArgument.push_back(3);
            auto rows = storage.get_all<User>(where(in(&User::id, inArgument)));
            REQUIRE(rows.size() == 3);
        }
    }
    {
        struct Letter {
            int id;
            std::string name;
        };
        auto storage = make_storage("",
                                    make_table("letters",
                                               make_column("id", &Letter::id, primary_key()),
                                               make_column("name", &Letter::name)));
        storage.sync_schema();
        
        storage.replace(Letter{1, "A"});
        storage.replace(Letter{2, "B"});
        storage.replace(Letter{3, "C"});
        
        {
            auto letters = storage.get_all<Letter>(where(in(&Letter::id, {1, 2, 3})));
            REQUIRE(letters.size() == 3);
        }
        {
            auto rows = storage.select(columns(&Letter::name), where(in(&Letter::id, {1, 2, 3})));
            REQUIRE(rows.size() == 3);
        }
        {
            auto rows2 = storage.select(&Letter::name, where(in(&Letter::id, {1, 2, 3})));
            REQUIRE(rows2.size() == 3);
        }
    }
}
