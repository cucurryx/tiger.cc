#ifndef TIGER_CC_AST_H
#define TIGER_CC_AST_H

#include "token.h"
#include "../utils/stringfy.h"

#include <vector>
#include <memory>
#include <string>

#define DEFINE_PTR(x) using x##Ptr = std::unique_ptr<x>;
#define DEFINE_VEC(x) using x##Vec = std::vector<x>;

/**
 * @brief pre declarations
 */
class AstNode;
class Operator;

class Expr;
class PrimeExpr;

// basic expressions
class NilExpr;
class IntExpr;
class StrExpr;

// expression sequence
class ExprSeq;

// assignment
class Assignment;

// creation
class ArrayCreate;
class RecordCreate;
class ObjectNew;

class MethodCall;
class FnCall;

class UnaryExpr;
class Exprs;

// statements
class IfStmt;
class WhileStmt;
class ForStmt;
class BreakStmt;
class LetStmt;

// lvalue
class Elem;
class Lvar;

// declarations
class Dec;

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


/**
 * @brief type. Although using macro can make these
 * `using and pre-declaration` shorter, I don't like it.
 */
using IdPtr = std::unique_ptr<Identifier>;

DEFINE_PTR(TypeId);
DEFINE_PTR(AstNode);
DEFINE_PTR(Operator);
DEFINE_PTR(Expr);
DEFINE_PTR(PrimeExpr);
DEFINE_PTR(NilExpr);
DEFINE_PTR(IntExpr);
DEFINE_PTR(StrExpr);
DEFINE_PTR(ArrayCreate);
DEFINE_PTR(RecordCreate);
DEFINE_PTR(ObjectNew);
DEFINE_PTR(FnCall);
DEFINE_PTR(UnaryExpr);
DEFINE_PTR(Exprs);
DEFINE_PTR(IfStmt);
DEFINE_PTR(WhileStmt);
DEFINE_PTR(ForStmt);
DEFINE_PTR(BreakStmt);
DEFINE_PTR(LetStmt);
DEFINE_PTR(Lvar);
DEFINE_PTR(Dec);
DEFINE_PTR(ClassField);
DEFINE_PTR(ClassFields);
DEFINE_PTR(Identifier);
DEFINE_PTR(Type);
DEFINE_PTR(TypeId);
DEFINE_PTR(TypeFields);
DEFINE_PTR(VarDec);
DEFINE_PTR(Decs);
DEFINE_PTR(MethodDec);
DEFINE_PTR(AttrDec);
DEFINE_PTR(FnDec);
DEFINE_PTR(PrimDec);
DEFINE_PTR(ImportDec);
DEFINE_PTR(TypeAlias);
DEFINE_PTR(RecordDef);
DEFINE_PTR(ArrayDef);
DEFINE_PTR(ClassDef);
DEFINE_PTR(ClassTypeDef);
DEFINE_PTR(ExprSeq);
DEFINE_PTR(Assignment);
DEFINE_PTR(MethodCall);
DEFINE_PTR(Elem);

DEFINE_VEC(IdPtr);
DEFINE_VEC(TypeIdPtr);
DEFINE_VEC(ExprPtr);
DEFINE_VEC(DecPtr);
DEFINE_VEC(ClassFieldPtr);
DEFINE_VEC(TypePtr);
DEFINE_VEC(OperatorPtr);
DEFINE_VEC(ElemPtr);

/**
 * @brief id
 */
class Identifier: public Stringfy {
public:
    explicit Identifier(std::string name): name_(std::move(name)) {}
    ~Identifier() = default;
    std::string ToString(u32 depth);

private:
    std::string name_;
};

class Operator: public Stringfy {
public:
    Operator(std::string op_): op_(std::move(op_)) {}
    ~Operator() = default;
    std::string ToString(u32 depth);

private:
    std::string op_;
    u64 precedence_;
};

class TypeId: public Stringfy {
public:
    explicit TypeId(std::string name): name_(std::move(name)) {}
    ~TypeId() = default;
    std::string ToString(u32 depth);

private:
    std::string name_;
};

class AstNode: public Stringfy {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
    virtual std::string ToString(u32 depth) {
        return "AstNode()";
    }
};

class Expr: public AstNode {
public:
    Expr(PrimeExprPtr left, OperatorPtrVec ops, ExprPtrVec rights):
        left_(std::move(left)),
        ops_(std::move(ops)),
        rights_(std::move(rights)) {
        assert(ops_.size() == rights_.size());
    }
    
    ~Expr() final = default;
    std::string ToString(u32 depth) final;

private:
    PrimeExprPtr left_;
    OperatorPtrVec ops_;
    ExprPtrVec rights_;
};

class PrimeExpr: public AstNode {
public:
    PrimeExpr() = default;
    virtual ~PrimeExpr() override = default;
    virtual std::string ToString(u32 depth) override;
};

// nil expression
class NilExpr: public PrimeExpr {
public:
    NilExpr() = default;
    ~NilExpr() final = default;
    std::string ToString(u32 depth) final;
};

// integer expression
class IntExpr: public PrimeExpr {
public:
    IntExpr(i64 num): num_(num) {}
    ~IntExpr() final = default;
    std::string ToString(u32 depth) final;

private:
    i64 num_;
};

// unary expression
class UnaryExpr: public PrimeExpr {
public:
    UnaryExpr(OperatorPtr op, ExprPtr expr):
        op_(std::move(op)), expr_(std::move(expr)) {}
    ~UnaryExpr() final = default;
    std::string ToString(u32 depth) final;

private:
    OperatorPtr op_;
    ExprPtr expr_;
};

// string expression
class StrExpr: public PrimeExpr {
public:
    StrExpr(std::string s): str_(std::move(s)) {}
    ~StrExpr() final = default;
    std::string ToString(u32 depth) final;

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

    ~ArrayCreate() final = default;
    std::string ToString(u32 depth) final;

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

    ~RecordCreate() final = default;
    std::string ToString(u32 depth) final;

private:
    TypeIdPtr type_id_;
    TypeIdPtrVec types_;
    ExprPtrVec vars_;
};

class Elem: public AstNode {
public:
    Elem(IdPtr name, ExprPtrVec idxs):
        name_(std::move(name)), idxs_(std::move(idxs)) {}
    std::string ToString(u32 depth);
private:
    IdPtr name_;
    ExprPtrVec idxs_;
};

class Lvar: public PrimeExpr {
public:
    Lvar(ElemPtrVec elems): elems_(std::move(elems)) {}
    std::string ToString(u32 depth);

private:
    ElemPtrVec elems_;
};

class ObjectNew: public PrimeExpr {
public:
    explicit ObjectNew(TypeIdPtr type): type_(std::move(type)) {}
    std::string ToString(u32 depth) final;

private:
    TypeIdPtr type_;
};

class FnCall: public PrimeExpr {
public:
    FnCall(IdPtr name, ExprPtrVec args):
        name_(std::move(name)),
        args_(std::move(args)) {}

    ~FnCall() final = default;
    std::string ToString(u32 depth) final;

private:
    IdPtr name_;
    ExprPtrVec args_;
};

class MethodCall: public PrimeExpr {
public:
    MethodCall(LvarPtr lvar, IdPtr method, ExprPtrVec args):
        lvar_(std::move(lvar)),
        method_(std::move(method)),
        args_(std::move(args)) {}

    ~MethodCall() final = default;
    std::string ToString(u32 depth) final;

private:
    LvarPtr lvar_;
    IdPtr method_;
    ExprPtrVec args_;
};

class Exprs {
public:
    Exprs(ExprPtrVec exprs):
        exprs_(std::move(exprs)) {}

    ~Exprs() = default;
    std::string ToString(u32 depth);

private:
    ExprPtrVec exprs_;
};

class ExprSeq: public PrimeExpr {
public:
    explicit ExprSeq(ExprsPtr exprs): exprs_(std::move(exprs)) {}
    std::string ToString(u32 depth) final;

private:
    ExprsPtr exprs_;
};

class Assignment: public PrimeExpr {
public:
    Assignment(LvarPtr lvar, ExprPtr expr):
        lval_(std::move(lvar)),
        expr_(std::move(expr)) {}

    std::string ToString(u32 depth) final;

private:
    LvarPtr lval_;
    ExprPtr expr_;
};

class IfStmt: public PrimeExpr {
public:
    IfStmt(ExprPtr _if, ExprPtr _then, ExprPtr _else):
        if_(std::move(_if)),
        then_(std::move(_then)),
        else_(std::move(_else)) {}

    ~IfStmt() final = default;
    std::string ToString(u32 depth) final;

private:
    ExprPtr if_;
    ExprPtr then_;
    ExprPtr else_;
};

class WhileStmt: public PrimeExpr {
public:
    WhileStmt(ExprPtr _while, ExprPtr _do):
        while_(std::move(_while)),
        do_(std::move(_do)) {}

    ~WhileStmt() final = default;
    std::string ToString(u32 depth) final;

private:
    ExprPtr while_;
    ExprPtr do_;
};

class ForStmt: public PrimeExpr {
public:
    ForStmt(IdPtr id, ExprPtr from, ExprPtr to, ExprPtr _do):
        id_(std::move(id)),
        from_(std::move(from)),
        to_(std::move(to)),
        do_(std::move(_do)) {}

    ~ForStmt() final = default;
    std::string ToString(u32 depth) final;

private:
    IdPtr id_;
    ExprPtr from_;
    ExprPtr to_;
    ExprPtr do_;
};

class BreakStmt: public PrimeExpr {
public:
    BreakStmt() = default;
    ~BreakStmt() final = default;
    std::string ToString(u32 depth) final;
};

class LetStmt: public PrimeExpr {
public:
    LetStmt(DecsPtr decs, ExprsPtr exprs):
        decs_(std::move(decs)),
        exprs_(std::move(exprs)) {}

    ~LetStmt() final = default;
    std::string ToString(u32 depth) final;

private:
    DecsPtr decs_;
    ExprsPtr exprs_;
};

// decs
class Dec: public AstNode {
public:
    Dec() = default;
    virtual ~Dec() override = default;
    virtual std::string ToString(u32 depth) {
        return "Dec()";
    }
};

class Decs: public AstNode {
public:
    Decs(DecPtrVec decs): decs_(std::move(decs)) {}
    ~Decs() final = default;
    std::string ToString(u32 depth) final;

private:
    DecPtrVec decs_;
};

// type declaration
class TypeDec: public Dec {
public:
    TypeDec(IdPtr name, TypePtr type):
        name_(std::move(name)),
        type_(std::move(type)) {}

    ~TypeDec() final = default;
    std::string ToString(u32 depth) final;

private:
    IdPtr name_;
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
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth);

private:
    std::string import_;
};

class ClassField: public AstNode {
public:
    ClassField() = default;
    virtual ~ClassField() = default;
    virtual std::string ToString(u32 depth);
};

class ClassFields: public AstNode {
public:
    explicit ClassFields(ClassFieldPtrVec fields):
        fields_(std::move(fields)) {}
    ~ClassFields() = default;
    std::string ToString(u32 depth);

private:
    ClassFieldPtrVec fields_;
};

// class field: attribute declaration 
class AttrDec: public ClassField {
public:
    AttrDec(VarDecPtr attr):
        attr_(std::move(attr)) {}
        
    ~AttrDec() = default;
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth);

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
    virtual std::string ToString(u32 depth);
};

class TypeAlias: public Type {
public:
    TypeAlias(TypeIdPtr alias): 
        alias_(std::move(alias)) {}
    ~TypeAlias() = default;
    std::string ToString(u32 depth);

private:
    TypeIdPtr alias_;
};

class RecordDef: public Type {
public:
    RecordDef(TypeFieldsPtr records):
        records_(std::move(records)) {}
    ~RecordDef() = default;
    std::string ToString(u32 depth);

private:
    TypeFieldsPtr records_;
};

class ArrayDef: public Type {
public:
    ArrayDef(TypeIdPtr type):
        type_(std::move(type)) {}
    ~ArrayDef() = default;
    std::string ToString(u32 depth);

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
    std::string ToString(u32 depth) final;

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
    ~TypeFields() final = default;
    std::string ToString(u32 depth) final;

private:
    IdPtrVec names_;
    TypeIdPtrVec types_;
};


#endif // TIGER_CC_AST_H