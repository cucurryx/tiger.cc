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
    ExprPtr ParseBinaryExpr(u64 expr_prec, ExprPtr lhs);
    PrimeExprPtr ParseExprTail();
    LvarPtr ParseLvar(ElemPtr elem);
    ElemPtr ParseElem();
    ExprPtr ParsePrimeExpr();
    UnaryExprPtr ParseUnaryExpr();

    // literal expressions
    NilExprPtr ParseNilExpr();
    IntExprPtr ParseIntExpr();
    StrExprPtr ParseStrExpr();

    // array and record creation expressions
    RecordCreatePtr ParseRecordCrt();

    // object creation expressions
    ObjectNewPtr ParseObjectNew();

    //left values


    // function and method call
    FnCallPtr ParseFnCall();

    // control expressions
    IfStmtPtr ParseIf();
    WhileStmtPtr ParseWhile();
    ForStmtPtr ParseFor();
    BreakStmtPtr ParseBreak();
    LetStmtPtr ParseLet();
    ExprSeqPtr ParseExprSeq();
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
};

#endif // TIGER_CC_PARSER_H