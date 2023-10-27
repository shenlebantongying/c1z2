#include "sexpr_rec.h"
#include <iostream>

namespace S {

std::vector<std::string>* tokenize(std::string_view str)
{
    auto* r = new std::vector<std::string>();

    std::string temp {};
    for (char a : str) {
        if (a == '(') {
            if (!temp.empty()) {
                r->emplace_back(temp.data());
                temp.clear();
            }
            r->emplace_back("(");
        } else if (a == ')') {
            if (!temp.empty()) {
                r->push_back(temp);
                temp.clear();
            }
            r->emplace_back(")");
        } else if (isspace(a)) {
            r->push_back(temp);
            temp.clear();
        } else {
            temp.push_back(a);
        }
    }

    return r;
}

std::tuple<Expr*, int> parse(std::vector<std::string>* tokens, size_t init_pos)
{
    size_t pos = init_pos;
    Expr* head = nullptr;
    Expr* cur = nullptr;

    while (pos < tokens->size()) {
        Expr* node;

        if (tokens->at(pos) == ")") {
            pos++;
            break;
        } else if (tokens->at(pos) == "(") {
            node = new Expr();
            pos++;
            auto [t, n] = parse(tokens, pos);
            node->val = t;
            pos += n;
        } else {
            node = make_String(tokens->at(pos));
            pos++;
        }

        if (head == nullptr) {
            head = node;
            cur = head;
        } else {
            cur->next = node;
            cur = cur->next;
        }
    }

    return { head, pos - init_pos };
}

Expr* cons(Expr* hd, Expr* tl)
{
    auto* r = new Expr();
    hd->next = tl;
    r->val = hd;
    return r;
}

std::string to_std(Expr* e)
{
    Expr* cur = e;
    std::string r;
    while (cur != nullptr) {
        if (string_q(cur)) {
            r += ' ' + get_String(cur) + ' ';
        } else if (list_q(cur)) {
            r += '(' + to_std(get_List(cur)) + ") ";
        }
        cur = cur->next;
    }
    return r;
}

void pretty_print(Expr* e, int ident)
{
    Expr* cur = e;
    while (cur != nullptr) {
        if (string_q(cur)) {
            for (int i = 0; i < ident; ++i) {
                std::cout << '\t';
            }
            std::cout << get_String(cur) << std::endl;
        } else if (list_q(cur)) {
            pretty_print(get_List(cur), ident + 1);
        }
        cur = cur->next;
    }
}

Expr* load(std::string_view str)
{
    auto [r, _] = parse(tokenize(str));
    return r;
}

Expr* make_String(std::string_view s)
{
    Expr* r = new Expr();
    r->val = s.data();
    return r;
}

std::string get_String(Expr* e)
{
    return std::get<std::string>(e->val);
}

Expr* get_List(Expr* e)
{
    return std::get<Expr*>(e->val);
}

Expr* cdr(Expr* e)
{
    return e->next;
}

bool string_q(Expr* e)
{
    return std::holds_alternative<std::string>(e->val);
}

bool atom_q(Expr* e)
{
    return string_q(e);
}

bool list_q(Expr* e)
{
    return std::holds_alternative<Expr*>(e->val);
}

} // namespace S
