#ifndef TIGER_CC_ENV_H
#define TIGER_CC_ENV_H

#include "symbol.h"
#include "ast.h"

#include <list>
#include <unordered_map>

template <typename T>
class EnvTable;

using SymbolTable = EnvTable<AstNodePtr>;
using TypeTable = EnvTable<TypePtr>;

template <typename T>
class EnvTable {
public:
    using ValuePtr = std::shared_ptr<T>;
    using ScopeMap = std::unordered_map<SymbolPtr, ValuePtr>;
    using ScopeList = std::list<ScopeMap>;

public:
    void BeginScope() {
        auto new_scope = GetCurrScope();
        scopes_.push_back(std::move(new_scope));
        ++scope_cnt_;
    }

    void EndScope() {
        if (!scopes_.empty()) {
            scopes_.pop_back();
        }
    }

    bool Remove(SymbolPtr symbol) {
        auto curr_scope = GetCurrScope();
        auto it = curr_scope.find(symbol);
        if (it != curr_scope.end()) {
            curr_scope.erase(it);
        }
    }

    bool Exist(SymbolPtr symbol) {
        return Find(symbol).get() != nullptr;
    }

    ValuePtr Find(SymbolPtr symbol) {
        auto curr_scope = GetCurrScope();
        auto it = curr_scope.find(symbol);
        if (it != curr_scope.end()) {
            return it->second;
        } else {
            return ValuePtr();
        }
    }

    void Add(SymbolPtr symbol, ValuePtr value) {
        if (!scopes_.empty()) {
            auto curr_scope = GetCurrScope();
            curr_scope[symbol] = value;
        }
    }

    ScopeMap GetCurrScope() {
        if (scopes_.empty()) {
            return ScopeMap();
        } else {
            return scopes_.back();
        }
    }

private:
    ScopeList scopes_;
    u64 scope_cnt_ {0};
};

#endif // TIGER_CC_ENV_H