#ifndef TIGER_CC_AST_H
#define TIGER_CC_AST_H

#include "token.h"

#include <vector>
#include <memory>
#include <string>

/**
 * @brief pre declarations
 */
class AstNode;
class Expr;
class Dec;
class Var;
class ClassField;
class Identifier;
class Type;
class TypeFields;
class VarDec;

/**
 * @brief type
 */
using TypeId = Identifier;
using IdPtr = std::unique_ptr<Identifier>;
using IdPtrVec = std::vector<IdPtr>;
using TypeIdPtr = IdPtr;
using TypeIdPtrVec = std::vector<TypeIdPtr>;
using AstNodePtr = std::unique_ptr<AstNode>;
using ExprPtr = std::unique_ptr<Expr>;
using DecPtr = std::unique_ptr<Dec>;
using ClassFieldPtr = std::unique_ptr<ClassField>;
using ExprPtrVec = std::vector<ExprPtr>;
using DecPtrVec = std::vector<DecPtr>;
using ClassFieldPtrVec = std::vector<ClassFieldPtr>;
using VarPtr = std::unique_ptr<Var>;
using OperatorPtr = std::unique_ptr<Operator>;
using TypePtr = std::unique_ptr<Type>;
using TypePtrVec = std::vector<TypePtr>;
using TypeFieldsPtr = std::unique_ptr<TypeFields>;
using VarDecPtr = std::unique_ptr<VarDec>;


class Identifier {
public:
    Identifier(std::string name): name_(std::move(name)) {}
private:
    std::string name_;
};

class AstNode {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
};

class Expr: public AstNode {
public:
    Expr() = default;
    virtual ~Expr() = default;
};

// nil expression
class NilExpr: public Expr {
public:
    NilExpr() = default;
    ~NilExpr() = default;
};

// integer expression
class IntExpr: public Expr {
public:
    IntExpr(i64 num): num_(num) {}
    ~IntExpr() = default;

private:
    i64 num_;
};

// string expression
class StrExpr: public Expr {
public:
    StrExpr(std::string &&s): str_(std::move(s)) {}
    ~StrExpr() = default;

private:
    std::string str_;
};

// array creation
class ArrayCreateAst: public Expr {
public:
    ArrayCreateAst(TypeIdPtr type_id, ExprPtr len, ExprPtr init):
        type_id_(std::move(type_id)),
        len_(std::move(len)),
        init_(std::move(init)) {}

    ~ArrayCreateAst() = default;

private:
    TypeIdPtr type_id_;
    ExprPtr len_;
    ExprPtr init_;
};

class RecordCreateAst: public Expr {
public:
    RecordCreateAst(TypeIdPtr type_id, TypeIdPtrVec types, ExprPtrVec vars):
        type_id_(std::move(type_id)),
        types_(std::move(types)),
        vars_(std::move(vars)) {
        assert(types_.size() == vars_.size());
    }

    ~RecordCreateAst() = default;

private:
    TypeIdPtr type_id_;
    TypeIdPtrVec types_;
    ExprPtrVec vars_;
};

class ObjCreateAst: public Expr {
public:
    ObjCreateAst(TypeIdPtr type_id): type_id_(std::move(type_id)) {}
    ~ObjCreateAst() = default;

private:
    TypeIdPtr type_id_;
};

class Var: public AstNode {
public:
    Var() = default;
    virtual ~Var() = default;
};

class BasicVar: public Var {

};

class FieldVar: public Var {

};

class ArrayElemVar: public Var {

};

// left value
class VarExpr: public Expr {
public:
    VarExpr(VarPtr var): lvar_(std::move(var)) {}
    ~VarExpr() = default;

private:
    VarPtr lvar_;
};

class FnCallAst: public Expr {
public:
    FnCallAst(IdPtr name, ExprPtrVec args):
        name_(std::move(name)),
        args_(std::move(args)) {}

    ~FnCallAst() = default;

private:
    IdPtr name_;
    ExprPtrVec args_;
};

class MethodCallAst: public Expr {
public:
    MethodCallAst(VarPtr lvar, IdPtr method, ExprPtrVec args):
        lvar_(std::move(lvar)),
        method_(std::move(method)),
        args_(std::move(args)) {}

    ~MethodCallAst() = default;

private:
    VarPtr lvar_;
    IdPtr method_;
    ExprPtrVec args_;
};

class OpExpr: public Expr {
public:
    OpExpr(OperatorPtr op, ExprPtr lhs, ExprPtr rhs):
        op_(std::move(op)),
        lhs_(std::move(lhs)),
        rhs_(std::move(rhs)) {}

    ~OpExpr() = default;

private:
    OperatorPtr op_;
    ExprPtr lhs_;
    ExprPtr rhs_;
};

class ExprsAst: public Expr {
public:
    ExprsAst(ExprPtrVec exprs):
        exprs_(std::move(exprs)) {}

    ~ExprsAst() = default;

private:
    ExprPtrVec exprs_;
};

class AssignExpr: public Expr {
public:
    AssignExpr(VarPtr lvar, ExprPtr expr):
        lval_(std::move(lvar)),
        expr_(std::move(expr)) {}

    ~AssignExpr() = default;

private:
    VarPtr lval_;
    ExprPtr expr_;
};

class IfExpr: public Expr {
public:
    IfExpr(ExprPtr _if, ExprPtr _then, ExprPtr _else):
        if_(std::move(_if)),
        then_(std::move(_then)),
        else_(std::move(_else)) {}

    ~IfExpr() = default;

private:
    ExprPtr if_;
    ExprPtr then_;
    ExprPtr else_;
};

class WhileExpr: public Expr {
public:
    WhileExpr(ExprPtr _while, ExprPtr _do):
        while_(std::move(_while)),
        do_(std::move(_do)) {}

    ~WhileExpr() = default;

private:
    ExprPtr while_;
    ExprPtr do_;
};

class ForExpr: public Expr {
public:
    ForExpr(IdPtr id, ExprPtr from, ExprPtr to, ExprPtr _do):
        id_(std::move(id)),
        from_(std::move(from)),
        to_(std::move(to)),
        do_(std::move(_do)) {}

    ~ForExpr() = default;

private:
    IdPtr id_;
    ExprPtr from_;
    ExprPtr to_;
    ExprPtr do_;
};

class BreakExpr: public Expr {
public:
    BreakExpr() = default;
    ~BreakExpr() = default;
};

class ExprList: public AstNode {
public:
    ExprList(ExprPtrVec exprs):
        exprs_(std::move(exprs)) {}

    ~ExprList() = default;

private:
    ExprPtrVec exprs_;
};

class LetExpr: public Expr {
public:
    LetExpr(DecPtrVec decs, ExprPtrVec exprs):
        decs_(std::move(decs)),
        exprs_(std::move(exprs)) {}

    ~LetExpr() = default;

private:
    DecPtrVec decs_;
    ExprPtrVec exprs_;
};

// decs
class Dec: public AstNode {
public:
    Dec() = default;
    virtual ~Dec() = default;
};

class DecList: public AstNode {
public:
    DecList(DecPtrVec decs):
        decs_(std::move(decs)) {}
    ~DecList() = default;

private:
    DecPtrVec decs_;
};

// type declaration
class TypeDec: public Dec {
public:
    TypeDec(IdPtr name, TypePtr type):
        name(std::move(name)),
        type_(std::move(type)) {}

    ~TypeDec() = default;

private:
    IdPtr name;
    TypePtr type_;
};

// class definition(alternative form)
class ClassDef: public Dec {
public:
    ClassDef(IdPtr name,
            TypeIdPtr parent, 
            ClassFieldPtrVec fields):
        name_(std::move(name)),
        parent_(std::move(parent)),
        fields_(std::move(fields)) {}

    ~ClassDef() = default;

private:
    IdPtr name_;
    TypeIdPtr parent_;
    ClassFieldPtrVec fields_;
};

// variable declaration
class VarDec: public Dec {
public:
    VarDec(IdPtr name, TypeIdPtr type, ExprPtr var):
        name_(std::move(name)),
        type_(std::move(type)),
        var_(std::move(var)) {}
        
     ~VarDec() = default;
    
private:
    IdPtr name_;
    TypeIdPtr type_;
    ExprPtr var_;
};

// functions declaration
class FnDec: public Dec {
public:
    FnDec(IdPtr name, TypeFieldsPtr args, TypeIdPtr ret, ExprPtr body):
        name_(std::move(name)),
        args_(std::move(args)),
        ret_(std::move(ret)),
        body_(std::move(body)) {}

    ~FnDec() = default;

private:
    IdPtr name_;
    TypeFieldsPtr args_;
    TypeIdPtr ret_;
    ExprPtr body_;
};

// primitive declaration
class PrimDec: public Dec {
public:
    PrimDec(IdPtr name, TypeFieldsPtr args, TypeIdPtr ret):
        name_(std::move(name)),
        args_(std::move(args)),
        ret_(std::move(ret)) {}

    ~PrimDec() = default;

private:
    IdPtr name_;
    TypeFieldsPtr args_;
    TypeIdPtr ret_;
};

// import declaration
class ImportDec: public Dec {
public:
    ImportDec(std::string import_):
        import_(std::move(import_)) {}

    ~ImportDec() = default;

private:
    std::string import_;
};

class ClassField: public AstNode {
public:
    ClassField() = default;
    virtual ~ClassField() = default;
};

// class field: attribute declaration 
class AttrDec: public ClassField {
public:
    AttrDec(VarDecPtr attr):
        attr_(std::move(attr)) {}
        
    ~AttrDec() = default;
    
private:
    VarDecPtr attr_;
};

// class field: method declaration
class MethodDec: public ClassField {
public:
    MethodDec(IdPtr name, TypeFieldsPtr args, TypeIdPtr ret, ExprPtr body):
        name_(std::move(name)),
        args_(std::move(args)),
        ret_(std::move(ret)),
        body_(std::move(body)) {}
        
    ~MethodDec() = default;
    
private:
    IdPtr name_;
    TypeFieldsPtr args_;
    TypeIdPtr ret_;
    ExprPtr body_;
};

class Type: public AstNode {
public:
    Type() = default;
    virtual ~Type() = default;
};

class TypeAlias: public Type {
public:
    TypeAlias(TypeIdPtr alias): 
        alias_(std::move(alias)) {}
    ~TypeAlias() = default;
private:
    TypeIdPtr alias_;
};

class RecordTypeDef: public Type {
public:
    RecordTypeDef(TypeFieldsPtr records):
        records_(std::move(records)) {}
    ~RecordTypeDef() = default;
private:
    TypeFieldsPtr records_;
};

class ArrayTypeDef: public Type {
public:
    ArrayTypeDef(TypeIdPtr type):
        type_(std::move(type)) {}
    ~ArrayTypeDef() = default;
private:
    TypeIdPtr type_;
};

// class definition(canonical form)
class ClassTypeDef: public Type {
public:
    ClassTypeDef(TypeIdPtr parent, ClassFieldPtrVec fields):
        parent_(std::move(parent)),
        fields_(std::move(fields)) {}
        
    ~ClassTypeDef() = default;
    
private:
    TypeIdPtr parent_;
    ClassFieldPtrVec fields_;
};

// type fields
class TypeFields: public AstNode {
public:
    TypeFields(IdPtrVec names, TypeIdPtrVec types):
        names_(std::move(names)),
        types_(std::move(types)) {}
    ~TypeFields() = default;

private:
    IdPtrVec names_;
    TypeIdPtrVec types_;
};


#endif // TIGER_CC_AST_H