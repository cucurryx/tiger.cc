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
class Operator;
class PrimeExpr;
class Dec;
class Var;
class VarA;
class ClassField;
class ClassFields;
class Identifier;
class Type;
class TypeId;
class TypeFields;
class VarDec;
class Decs;
class MethodDec;
class AttrDec;
class FnDec;
class PrimDec;
class ImportDec;
class TypeAlias;
class RecordDef;
class ArrayDef;
class ClassDef;
class ClassTypeDef;
class NilExpr;
class IntExpr;
class StrExpr;
class ObjCreate;
class ArrayCreate;
class RecordCreate;
class BasicVar;
class FieldVar;
class ArrayElemVar;
class FnCall;
class MethodCall;
class OpExpr;
class AssignExpr;
class IfExpr;
class WhileExpr;
class ForExpr;
class BreakExpr;
class LetExpr;
class Exprs;
class VarExpr;
class ExprsExpr;
class UnaryExpr;

/**
 * @brief type. Although using macro can make these
 * `using and pre-declaration` shorter, I don't like it.
 */
using IdPtr = std::unique_ptr<Identifier>;
using IdPtrVec = std::vector<IdPtr>;
using TypeIdPtr = std::unique_ptr<TypeId>;
using TypeIdPtrVec = std::vector<TypeIdPtr>;
using AstNodePtr = std::unique_ptr<AstNode>;
using ExprPtr = std::unique_ptr<Expr>;
using PrimeExprPtr = std::unique_ptr<PrimeExpr>;
using DecPtr = std::unique_ptr<Dec>;
using ClassFieldPtr = std::unique_ptr<ClassField>;
using ExprPtrVec = std::vector<ExprPtr>;
using DecPtrVec = std::vector<DecPtr>;
using ClassFieldPtrVec = std::vector<ClassFieldPtr>;
using VarPtr = std::unique_ptr<Var>;
using VarAPtr = std::unique_ptr<VarA>;
using OperatorPtr = std::unique_ptr<Operator>;
using TypeFieldsPtr = std::unique_ptr<TypeFields>;
using VarDecPtr = std::unique_ptr<VarDec>;
using DecsPtr = std::unique_ptr<Decs>;
using ClassFieldsPtr = std::unique_ptr<ClassFields>;
using MethodDecPtr = std::unique_ptr<MethodDec>;
using AttrDecPtr = std::unique_ptr<AttrDec>;
using FnDecPtr = std::unique_ptr<FnDec>;
using PrimDecPtr = std::unique_ptr<PrimDec>;
using ImportDecPtr = std::unique_ptr<ImportDec>;
using NilExprPtr = std::unique_ptr<NilExpr>;
using IntExprPtr = std::unique_ptr<IntExpr>;
using StrExprPtr = std::unique_ptr<StrExpr>;
using ObjCreatePtr = std::unique_ptr<ObjCreate>;
using RecordCreatePtr = std::unique_ptr<RecordCreate>;
using ArrayCreatePtr = std::unique_ptr<ArrayCreate>;
using BasicVarPtr = std::unique_ptr<BasicVar>;
using ArrayElemVarPtr = std::unique_ptr<ArrayElemVar>;
using FieldVarPtr = std::unique_ptr<FieldVar>;
using FnCallPtr = std::unique_ptr<FnCall>;
using MethodCallPtr = std::unique_ptr<MethodCall>;
using OpExprPtr = std::unique_ptr<OpExpr>;
using OpExprPtrVec = std::vector<OpExprPtr>;
using AssignExprPtr = std::unique_ptr<AssignExpr>;
using IfExprPtr = std::unique_ptr<IfExpr>;
using WhileExprPtr = std::unique_ptr<WhileExpr>;
using ForExprPtr = std::unique_ptr<ForExpr>;
using BreakExprPtr = std::unique_ptr<BreakExpr>;
using LetExprPtr = std::unique_ptr<LetExpr>;
using ExprsPtr = std::unique_ptr<Exprs>;
using VarExprPtr = std::unique_ptr<VarExpr>;
using ExprsExprPtr = std::unique_ptr<ExprsExpr>;
using OpPtr = std::unique_ptr<Operator>;
using OpPtrVec = std::vector<OpPtr>;
using UnaryExprPtr = std::unique_ptr<UnaryExpr>;

// types
using TypePtr = std::unique_ptr<Type>;
using TypePtrVec = std::vector<TypePtr>;
using TypeAliasPtr = std::unique_ptr<TypeAlias>;
using RecordDefPtr = std::unique_ptr<RecordDef>;
using ClassTypeDefPtr = std::unique_ptr<ClassTypeDef>;
using ArrayDefPtr = std::unique_ptr<ArrayDef>;

/**
 * @brief id
 */
class Identifier {
public:
    explicit Identifier(std::string name): name_(std::move(name)) {}
private:
    std::string name_;
};

class Operator {
public:
    Operator(std::string op_): op_(std::move(op_)) {}
    ~Operator() = default;

private:
    std::string op_;
    u64 precedence_;
};

class TypeId {
public:
    explicit TypeId(std::string name): name_(std::move(name)) {}
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
    Expr(PrimeExprPtr left, OpPtrVec ops, ExprPtrVec rights):
        left_(std::move(left)),
        ops_(std::move(ops)),
        rights_(std::move(rights)) {
        assert(ops_.size() == rights_.size());
    }
    
    ~Expr() = default;

private:
    PrimeExprPtr left_;
    OpPtrVec ops_;
    ExprPtrVec rights_;
};

class PrimeExpr: public AstNode {
public:
    PrimeExpr() = default;
    virtual ~PrimeExpr() = default;
};

// nil expression
class NilExpr: public PrimeExpr {
public:
    NilExpr() = default;
    ~NilExpr() = default;
};

// integer expression
class IntExpr: public PrimeExpr {
public:
    IntExpr(i64 num): num_(num) {}
    ~IntExpr() = default;

private:
    i64 num_;
};

// unary expression
class UnaryExpr: public PrimeExpr {
public:
    UnaryExpr(OpPtr op, ExprPtr expr):
        op_(std::move(op)), expr_(std::move(expr)) {}
    ~UnaryExpr() = default;

private:
    OpPtr op_;
    ExprPtr expr_;
};

// string expression
class StrExpr: public PrimeExpr {
public:
    StrExpr(std::string s): str_(std::move(s)) {}
    ~StrExpr() = default;

private:
    std::string str_;
};

// array creation
class ArrayCreate: public PrimeExpr {
public:
    ArrayCreate(TypeIdPtr type_id, ExprPtr len, ExprPtr init):
        type_id_(std::move(type_id)),
        len_(std::move(len)),
        init_(std::move(init)) {}

    ~ArrayCreate() = default;

private:
    TypeIdPtr type_id_;
    ExprPtr len_;
    ExprPtr init_;
};

class RecordCreate: public PrimeExpr {
public:
    RecordCreate(TypeIdPtr type_id, TypeIdPtrVec types, ExprPtrVec vars):
        type_id_(std::move(type_id)),
        types_(std::move(types)),
        vars_(std::move(vars)) {
        assert(types_.size() == vars_.size());
    }

    ~RecordCreate() = default;

private:
    TypeIdPtr type_id_;
    TypeIdPtrVec types_;
    ExprPtrVec vars_;
};

class ObjCreate: public PrimeExpr {
public:
    ObjCreate(TypeIdPtr type_id): type_id_(std::move(type_id)) {}
    ~ObjCreate() = default;

private:
    TypeIdPtr type_id_;
};

class Var: public AstNode {
public:
    Var(IdPtr id, VarAPtr rhs): 
        id_(std::move(id)), rhs_(std::move(rhs)) {}
    ~Var() = default;

private:
    IdPtr id_;
    VarAPtr rhs_;
};

class VarA: public AstNode {
public:
    VarA() = default;
    virtual ~VarA() = default;
};

class BasicVar: public VarA {
public:
    BasicVar() = default;
    ~BasicVar() = default;
};

class FieldVar: public VarA {
public:
    FieldVar(VarAPtr lvar, IdPtr name):
        lvar_(std::move(lvar)),
        name_(std::move(name)) {}
    ~FieldVar() = default;

private:
    VarAPtr lvar_;
    IdPtr name_;
};

class ArrayElemVar: public VarA {
public:
    ArrayElemVar(VarAPtr lvar, ExprPtr index):
        lvar_(std::move(lvar)),
        index_(std::move(index)) {}
    ~ArrayElemVar() = default;

private:
    VarAPtr lvar_;
    ExprPtr index_;
};

// left value
class VarExpr: public PrimeExpr {
public:
    VarExpr(VarPtr var): lvar_(std::move(var)) {}
    ~VarExpr() = default;

private:
    VarPtr lvar_;
};

class FnCall: public PrimeExpr {
public:
    FnCall(IdPtr name, ExprPtrVec args):
        name_(std::move(name)),
        args_(std::move(args)) {}

    ~FnCall() = default;

private:
    IdPtr name_;
    ExprPtrVec args_;
};

class MethodCall: public PrimeExpr {
public:
    MethodCall(VarPtr lvar, IdPtr method, ExprPtrVec args):
        lvar_(std::move(lvar)),
        method_(std::move(method)),
        args_(std::move(args)) {}

    ~MethodCall() = default;

private:
    VarPtr lvar_;
    IdPtr method_;
    ExprPtrVec args_;
};

class OpExpr: public PrimeExpr {
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

class Exprs {
public:
    Exprs(ExprPtrVec exprs):
        exprs_(std::move(exprs)) {}

    ~Exprs() = default;

private:
    ExprPtrVec exprs_;
};

class ExprsExpr: public PrimeExpr {
public:
    explicit ExprsExpr(ExprsPtr exprs): exprs_(std::move(exprs)) {}
    ~ExprsExpr() = default;

private:
    ExprsPtr exprs_;
};

class AssignExpr: public PrimeExpr {
public:
    AssignExpr(VarPtr lvar, ExprPtr expr):
        lval_(std::move(lvar)),
        expr_(std::move(expr)) {}

    ~AssignExpr() = default;

private:
    VarPtr lval_;
    ExprPtr expr_;
};

class IfExpr: public PrimeExpr {
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

class WhileExpr: public PrimeExpr {
public:
    WhileExpr(ExprPtr _while, ExprPtr _do):
        while_(std::move(_while)),
        do_(std::move(_do)) {}

    ~WhileExpr() = default;

private:
    ExprPtr while_;
    ExprPtr do_;
};

class ForExpr: public PrimeExpr {
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

class BreakExpr: public PrimeExpr {
public:
    BreakExpr() = default;
    ~BreakExpr() = default;
};

class LetExpr: public PrimeExpr {
public:
    LetExpr(DecsPtr decs, ExprsExprPtr exprs):
        decs_(std::move(decs)),
        exprs_(std::move(exprs)) {}

    ~LetExpr() = default;

private:
    DecsPtr decs_;
    ExprsExprPtr exprs_;
};

// decs
class Dec: public AstNode {
public:
    Dec() = default;
    virtual ~Dec() = default;
};

class Decs: public AstNode {
public:
    Decs(DecPtrVec decs): decs_(std::move(decs)) {}
    ~Decs() = default;

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
            ClassFieldsPtr fields):
        name_(std::move(name)),
        parent_(std::move(parent)),
        fields_(std::move(fields)) {}

    ~ClassDef() = default;

private:
    IdPtr name_;
    TypeIdPtr parent_;
    ClassFieldsPtr fields_;
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

class ClassFields: public AstNode {
public:
    explicit ClassFields(ClassFieldPtrVec fields):
        fields_(std::move(fields)) {}
    ~ClassFields() = default;

private:
    ClassFieldPtrVec fields_;
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

class RecordDef: public Type {
public:
    RecordDef(TypeFieldsPtr records):
        records_(std::move(records)) {}
    ~RecordDef() = default;
private:
    TypeFieldsPtr records_;
};

class ArrayDef: public Type {
public:
    ArrayDef(TypeIdPtr type):
        type_(std::move(type)) {}
    ~ArrayDef() = default;
private:
    TypeIdPtr type_;
};

// class definition(canonical form)
class ClassTypeDef: public Type {
public:
    ClassTypeDef(TypeIdPtr parent, ClassFieldsPtr fields):
        parent_(std::move(parent)),
        fields_(std::move(fields)) {}
        
    ~ClassTypeDef() = default;
    
private:
    TypeIdPtr parent_;
    ClassFieldsPtr fields_;
};

// type fields
class TypeFields: public AstNode {
public:
    TypeFields(IdPtrVec names, TypeIdPtrVec types):
        names_(std::move(names)),
        types_(std::move(types)) {
        assert(names_.size() == types_.size());
    }
    ~TypeFields() = default;

private:
    IdPtrVec names_;
    TypeIdPtrVec types_;
};


#endif // TIGER_CC_AST_H