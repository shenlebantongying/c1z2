#include "sexpr_rec.h"
#include <gtest/gtest.h>

TEST(S_parse, BasicAssertions)
{
    auto [res, n] = S::parse(S::tokenize("(abc def (hij osk))"));
    EXPECT_EQ(n, 8);
    EXPECT_EQ(S::get_String(
                  S::cdr(
                      S::get_List(
                          S::cdr(
                              S::cdr(
                                  S::get_List(res)))))),
        "osk");
}

TEST(S_cons_to_std, BasicAssertions)
{
    EXPECT_EQ(S::to_std(
                  S::cons(
                      S::make_String("a"),
                      S::cons(S::make_String("b"), S::make_String("c")))),
        "( a ( b  c ) ) ");
}