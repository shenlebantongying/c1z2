#pragma once

#include <cctype>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace S {

struct Expr {
    Expr() = default;
    Expr* next = nullptr;

    std::variant<std::string, Expr*> val;
};

// Processors

std::vector<std::string>* tokenize(std::string_view str);
std::tuple<Expr*, int> parse(std::vector<std::string>* tokens, size_t init_pos = 0);

Expr* load(std::string_view str);

// Predicates

bool atom_q(Expr* e);
bool string_q(Expr* e);
bool list_q(Expr* e);

// Creators

Expr* make_String(std::string_view s);
Expr* cons(Expr* hd, Expr* tl);

// Accessors
std::string get_String(Expr* e);
Expr* get_List(Expr* e);
Expr* cdr(Expr* e);

//
void pretty_print(Expr* e, int ident = -1);
std::string to_std(Expr* e);
} // namespace S