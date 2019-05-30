#ifndef TIGER_CC_AST_H
#define TIGER_CC_AST_H

#include <vector>
#include <memory>

/**
 * @brief pre declarations
 */
class AstNode;
class Expr;
class Dec;
class ClassField;

/**
 * @brief type
 */
using AstNodePtr = std::unique_ptr<AstNode>;
using ExprPtr = std::unique_ptr<Expr>;
using DecPtr = std::unique_ptr<Dec>;
using ClassFieldPtr = std::unique_ptr<ClassField>;
using ExprPtrVec = std::vector<ExprPtr>;
using DecPtrVec = std::vector<DecPtr>;
using ClassFieldPtrVec = std::vector<ClassFieldPtr>;

class AstNode {
public:
    AstNode() = default;
    virtual ~AstNode() = default;
};

// program ::= exp | decs
class ExprAst: public AstNode {

};

class NilExprAst: public ExprAst {

};

class IntExprAst: public ExprAst {

};

class StrExprAst: public ExprAst {

};

class ArrayCreateAst: public ExprAst {

};

class RecordCreateAst: public ExprAst {

};

class ObjCreateAst: public ExprAst {

};

class Var: public AstNode {

};

class BasicVar: public Var {

};

class FieldVar: public Var {

};

class ArrayElemVar: public Var {

};

class VarExprAst: public ExprAst {

};

class FnCallAst: public ExprAst {

};

class MethodCallAst: public ExprAst {

};

class OpExprAst: public ExprAst {

};

class AssignExprAst: public ExprAst {

};

class IfExprAst: public ExprAst {

};

class WhileExprAst: public ExprAst {

};

class ForExprAst: public ExprAst {

};

class BreakExprAst: public ExprAst {

};

class ExprList: public AstNode {

};

class LetExprAst: public ExprAst {

};

// decs
class Dec: public AstNode {

};

class DecList: public AstNode {

};

// type declaration
class TypeDec: public Dec {

};

// class definition(alternative form)
class ClassDef: public Dec {

};

// variable declaration
class VarDec: public Dec {

};

// functions declaration
class FnDec: public Dec {

};

// primitive declaration
class PrimDec: public Dec {

};

// import declaration
class ImportDec: public Dec {

};

class ClassField: public AstNode {

};

class AttrDec: public ClassField {

};

class MethodDec: public ClassField {

};

class ClassFields: public AstNode {

};

class Type: public AstNode {

};

class TypeAlias: public Type {

};

class RecordTypeDef: public Type {

};

class ArrayTypeDef: public Type {

};

// class definition(canonical form)
class ClassTypeDef: public Type {

};

// type fields
class TypeFields: public AstNode {

};

#endif // TIGER_CC_AST_H