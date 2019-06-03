#ifndef TIGER_CC_PARSER_H
#define TIGER_CC_PARSER_H

#include "lexer.h"
#include "ast.h"

#include <map>

class Parser {
public:
    template <typename T, typename... Args>
    std::unique_ptr<T> MakeUnique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

public:
    explicit Parser(Lexer::TokenPtrVec &&tokens):
        tokens_(FilterTokens(tokens)) {}

    AstNodePtr ParseResult();

private:
    static Lexer::TokenPtrVec FilterTokens(Lexer::TokenPtrVec tokens);

    Lexer::TokenPtr NextToken();
    Lexer::TokenPtr CurrToken();
    Lexer::TokenPtr Expect(Token::Tag tag);
    Lexer::TokenPtr NotNullNext();
    Lexer::TokenPtr PeekNext();
    bool CurrIs(Token::Tag tag);
    bool Try(Token::Tag tag);
    bool IsOperator(Token::Tag tag);

    AstNodePtr ParseMain();
    // expressions
    ExprPtr ParseTopExpr();
    PrimeExprPtr ParsePrimeExpr();
    UnaryExprPtr ParseUnaryExpr();

    // literal expressions
    NilExprPtr ParseNilExpr();
    IntExprPtr ParseIntExpr();
    StrExprPtr ParseStrExpr();

    // array and record creation expressions
    ArrayCreatePtr ParseArrayCrt();
    RecordCreatePtr ParseRecordCrt();

    // object creation expressions
    ObjCreatePtr ParseObjCrt();

    //left values
    VarPtr ParseVar();
    VarAPtr ParseVarA();
    VarExprPtr ParseVarExpr();
    BasicVarPtr ParseBasicVar();
    ArrayElemVarPtr ParseArrayElem();
    FieldVarPtr ParseFieldVar();

    // function and method call
    FnCallPtr ParseFnCall();
    MethodCallPtr ParseMethodCall();

    // operations and assignment
    OpExprPtr ParseOpExpr();
    AssignExprPtr ParseAssignExpr();

    // control expressions
    IfExprPtr ParseIf();
    WhileExprPtr ParseWhile();
    ForExprPtr ParseFor();
    BreakExprPtr ParseBreak();
    LetExprPtr ParseLet();
    ExprsExprPtr ParseExprsExpr();
    ExprsPtr ParseExprs();

    // declarations
    DecsPtr ParseDecs();
    DecPtr ParseDec();
    DecPtr ParseTypeDec();
    DecPtr ParseClassDefA();
    FnDecPtr ParseFnDec();
    PrimDecPtr ParsePrimDec();
    ImportDecPtr ParseImportDec();
    VarDecPtr ParseVarDec();
    ClassFieldsPtr ParseClassFields();
    ClassFieldPtr ParseClassField();
    MethodDecPtr ParseMethodDec();
    AttrDecPtr ParseAttrDec();
    TypeFieldsPtr ParseTypeFields();

    // type declarations
    TypePtr ParseType();
    TypeAliasPtr ParseAliasType();
    RecordDefPtr ParseRecordDef();
    ArrayDefPtr ParseArrayDef();
    ClassTypeDefPtr ParseClassTypeDef();

private:
    Lexer::TokenPtrVec tokens_;
    u32 index_ {0};
    static std::map<std::string, u64> op_prec_m_;
};

#endif // TIGER_CC_PARSER_H