#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Variant.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
struct Test{
    // Test(Test &&) = delete;
    // Test(const Test &) = default;
    // Test() = default;

    Test(const Test &) = default;
    Test() = default;
    ~Test() = default;

    Test operator+(const Test & other) const
    {
        Test tmp;
        tmp.a = a + other.a;
        return tmp;
    }

    int a = -1;
};


using Variant = OwO::Variant<string, int , double, Test, std::vector<int>>;

TEST_CASE("normal"){
    Variant var1;
    Variant var2;

    var1 = "test";
    var2 = var1;
    CHECK_EQ(var1.Get<string>(), var2.Get<string>());

    var1 = std::vector<int>{1,2,3,4};
    var2 = var1;
    CHECK_EQ(var1.Get<std::vector<int>>().size(), 4);
    CHECK_EQ(var2.Get<std::vector<int>>().size(), 4);
}

TEST_CASE("release"){
    Variant var1;
    Variant var2;
    std::string str = "tstadfsabsafsafwegwagfasfwegwfasfawsg";
    {
        Variant var = str;
        var1 = var;
        var2 = std::move(var);
    }
    CHECK_EQ(var1, str);
    CHECK_EQ(var1, var2);
}


TEST_CASE("string"){
    Variant var;
    var = 10;
    CHECK_EQ(var.String(), std::string("10"));

    var = "test";
    CHECK_EQ(var.String(), std::string("test"));

    var = 10.1;
    CHECK_EQ(var.String(), std::string("10.100000"));

    /* 子定义类型输出 */
    Variant::RegisterFormatString(typeid(Test), [](const void * pData){
        const Test * t = (const Test*) pData;
        return OwO::Format("a: %d", t->a);
    });

    Test a;
    a.a = 1000;
    var = a;
    CHECK_EQ(var.String(), std::string("a: 1000"));
}

TEST_CASE("custom struct"){
    Variant var1;
    Variant var2;

    Test a1;
    a1.a = 11;
    Test a2;
    a2.a = 22;

    Test a3 = a1 + a2;

    var1 = a1;
    var2 = a2;

    auto resAdd = var1 + var2;
    auto resMinus = var1 - var2;

    CHECK_EQ(resAdd.Get<Test>().a, 33);
    CHECK_EQ(resMinus.Get<Test>().a, -1);

}

TEST_CASE("four operator"){
    Variant var1;
    Variant var2;

    var1 = 4;

    var2 = var1 + 10;
    CHECK_EQ(var2.Get<int>(), 14);

    int res1 = var1 + 4;
    CHECK_EQ(res1, 8);

    bool bFlag = OwO::has_operator_divide<std::string>::value;

    var1 = 3;
    var2 = 2;
    Variant var3 = var2 + var1;
    CHECK_EQ(var3.Get<int>(), 5);
}

TEST_CASE("bool operator"){
    Variant var1;
    Variant var2;

    var1 = 10;
    var2 = 11;
    CHECK_EQ(var1 == var2, false);
    CHECK_EQ(var1 != var2, true);
    CHECK_EQ(var1 > var2, false);
    CHECK_EQ(var1 > 5, true);
    CHECK_EQ(var1 <= 10, true);

    var1 = "test1";
    var2 = "test2";
    Variant var3 = var1 + var2;
    CHECK_EQ(var1 == var2, false);
    CHECK_EQ(var1 != var2, true);
    CHECK_EQ(var1 > var2, false);
    CHECK_EQ(var1 < var2, true);
}


int main(int argc, char const *argv[])
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
