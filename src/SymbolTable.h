//
// Created by Fan Long on 2020/12/5.
//

#ifndef MINICC_SYMBOLTABLE_H
#define MINICC_SYMBOLTABLE_H

//add more header files if your want
//You may need assert function
#include <cassert>
#include "Types.h"
#include <map>
#include <iostream>

namespace llvm {
    class Value;
}

namespace minicc {


    struct VarSymbolEntry {
        Type VarType;
        llvm::Value *LLVMValue;

        explicit VarSymbolEntry(Type varType) : VarType(varType), LLVMValue(nullptr) { }
    };

    class VarSymbolTable {

        std::map<std::string, VarSymbolEntry> Table;

    public:
        //define your member variables and functions
        VarSymbolTable() : Table() { }
        void insert(const std::string &key, const VarSymbolEntry &entry) {
            Table.emplace(key, entry);
        }
    
        VarSymbolEntry* lookup(const std::string &key) const {
            auto it = Table.find(key);
            if (it != Table.end()) {
                return (VarSymbolEntry*)&it->second;
            }
            return nullptr;
        }
    };

    struct FuncSymbolEntry {
        Type ReturnType;
        std::vector<Type> ParameterTypes;
        bool HasBody;

        FuncSymbolEntry(Type retType, const std::vector<Type> &paraTypes, bool hasBody) : ReturnType(retType), ParameterTypes(paraTypes), HasBody(hasBody) { }
    };

    class FuncSymbolTable {
        std::map<std::string, FuncSymbolEntry> Table;
    public:
        //define your member variables and functions
        void insert(const std::string &key, const FuncSymbolEntry entry) {
            Table.emplace(key, entry);
        }
        FuncSymbolEntry* lookup(std::string key) {
            auto it = Table.find(key);
            if (it != Table.end()) {
                return &it->second;
            }
            return nullptr;
        }
    };
}

#endif //MINICC_SYMBOLTABLE_H
