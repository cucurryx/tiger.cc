#include "parser.h"
#include "../utils/error.h"

#include <string>

// mapping operator to it's precedence.
// comparison operators are not associative,
// others are left-associative
std::map<std::string, u64> op_prec_m_ = {
        {"*", 8},
        {"/", 8},
        {"+", 4},
        {"-", 4},
        {">=", 2},
        {"<=", 2},
        {"=", 2},
        {"<>", 2},
        {">", 2},
        {"<", 2},
        {"&", 1},
        {"|", 0},
};

AstNodePtr Parser::ParseResult() {
    return nullptr;
}

Lexer::TokenPtr Parser::NextToken() {
    if (index_ >= tokens_.size()) {
        return nullptr;
    }
    return tokens_[index_++];
}

Lexer::TokenPtr Parser::CurrToken() {
    if (index_ >= tokens_.size()) {
        return nullptr;
    }
    return tokens_[index_];
}

// if current token is null, panic.
// if not, eat current token and return it.
Lexer::TokenPtr Parser::NotNullNext() {
    auto curr = NextToken();
    if (curr == nullptr) {
        PANIC("need not null token")
    }
    return curr;
}

// peek the next token
Lexer::TokenPtr Parser::PeekNext() {
    if (index_ + 1 >= tokens_.size()) {
        return nullptr;
    }
    return tokens_[index_+1];
}

// expect current token's type is tag
Lexer::TokenPtr Parser::Expect(Token::Tag tag) {
    if (CurrToken() == nullptr) {
        PANIC("curr token is null")
    }
    if (CurrToken()->Type() != tag) {
        PANIC("not expected token's tag")
    }
    return NextToken();
}

// current token's type is tag
bool Parser::CurrIs(Token::Tag tag) {
    if (CurrToken() == nullptr) {
        PANIC("curr token is null")
    }
    return CurrToken()->Type() == tag;
}

// if current token's type is tag, eat current token and return true
// else return false.
bool Parser::Try(Token::Tag tag) {
    if (CurrToken() != nullptr &&
        CurrToken()->Type() == tag) {
        NextToken();
        return true;
    }
    return false;
}

// program ::= decs | exp
AstNodePtr Parser::ParseMain() {
    auto curr = CurrToken();
    if (curr == nullptr) {
        return nullptr;
    }

    auto result = AstNodePtr();
    switch (curr->Type()) {
        case Token::Tag::TYPE:
        case Token::Tag::CLASS:
        case Token::Tag::FUNCTION:
        case Token::Tag::PRIMITIVE:
        case Token::Tag::IMPORT:
        case Token::Tag::VAR:
            result = ParseDecs();
        default:
            result = ParseTopExpr();
    }

    return result;
}

DecsPtr Parser::ParseDecs() {
    auto decs = DecPtrVec();
    for (;;) {
        if (auto p = ParseDec(); p != nullptr) {
            decs.push_back(ParseDec());
        } else {
            break;
        }
    }
    return MakeUnique<Decs>(std::move(decs));
}

DecPtr Parser::ParseDec() {
    auto dec = DecPtr();
    auto token = NextToken();
    if (token == nullptr) {
        return nullptr;
    }

    switch (token->Type()) {
        case Token::Tag::TYPE:
            dec = ParseTypeDec();
            break;
        case Token::Tag::CLASS:
            dec = ParseClassDefA();
            break;
        case Token::Tag::FUNCTION:
            dec = ParseFnDec();
            break;
        case Token::Tag::PRIMITIVE:
            dec = ParsePrimDec();
            break;
        case Token::Tag::IMPORT:
            dec = ParseImportDec();
            break;
        case Token::Tag::VAR:
            dec = ParseVarDec();
            break;
        default:
            PANIC("invalid token type")
    }
    return dec;
}

DecPtr Parser::ParseTypeDec() {
    auto id = Expect(Token::Tag::ID);
    auto _ = Expect(Token::Tag::EQ);
    auto type = ParseType();
    auto name = MakeUnique<Identifier>(id->Value());
    return MakeUnique<TypeDec>(std::move(name), std::move(type));
}

DecPtr Parser::ParseClassDefA() {
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    auto parent = TypeIdPtr();

    if (CurrIs(Token::Tag::EXTENDS)) {
        NextToken(); // eat 'extends'
        auto p = Expect(Token::Tag::ID);
        parent = MakeUnique<TypeId>(p->Value());
    }

    Expect(Token::Tag::LPAREN);
    auto fields = ParseClassFields();
    Expect(Token::Tag::RPAREN);

    return MakeUnique<ClassDef>(std::move(name),
            std::move(parent), std::move(fields));
}

ClassFieldsPtr Parser::ParseClassFields() {
    auto fields = ClassFieldPtrVec();
    auto field = ClassFieldPtr();
    while ((field = ParseClassField()) != nullptr) {
        fields.push_back(std::move(field));
    }
    return MakeUnique<ClassFields>(std::move(fields));
}

// class-field ::= attr-dec | method-dec
ClassFieldPtr Parser::ParseClassField() {
    if (Try(Token::Tag::VAR)) {
        return ParseAttrDec();
    }
    if (Try(Token::Tag::METHOD)) {
        return ParseMethodDec();
    }
    PANIC("class field should be attribute or method")
    return nullptr;
}

// method declaration in class fields
// method id (type-fields) [ : type-id ] = exp
MethodDecPtr Parser::ParseMethodDec() {
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    Expect(Token::Tag::LPAREN);
    auto args = ParseTypeFields();
    Expect(Token::Tag::RPAREN);

    auto ret = TypeIdPtr();
    if (Try(Token::Tag::COLON)) {
        auto type_id = Expect(Token::Tag::ID);
        ret = MakeUnique<TypeId>(type_id->Value());
    }

    Expect(Token::Tag::EQ);
    auto body = ParseTopExpr();
    return MakeUnique<MethodDec>(std::move(name), std::move(args),
            std::move(ret), std::move(body));
}

// attribute declaration in class fields
AttrDecPtr Parser::ParseAttrDec() {
    return MakeUnique<AttrDec>(ParseVarDec());
}

VarDecPtr Parser::ParseVarDec() {
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    auto type = TypeIdPtr();

    if (CurrIs(Token::Tag::COLON)) {
        auto type_id = Expect(Token::Tag::ID);
        type = MakeUnique<TypeId>(type_id->Value());
    }

    Expect(Token::Tag::ASSIGN);
    auto body = ParseTopExpr();
    return MakeUnique<VarDec>(
            std::move(name), std::move(type), std::move(body));
}

FnDecPtr Parser::ParseFnDec() {
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());

    Expect(Token::Tag::LPAREN);
    auto args = ParseTypeFields();

    Expect(Token::Tag::RPAREN);
    auto ret = TypeIdPtr();

    if (CurrIs(Token::Tag::COLON)) {
        auto type_id = Expect(Token::Tag::ID);
        ret = MakeUnique<TypeId>(type_id->Value());
    }
    Expect(Token::Tag::EQ);

    auto body = ParseTopExpr();
    return MakeUnique<FnDec>(std::move(name), std::move(args),
            std::move(ret), std::move(body));
}

PrimDecPtr Parser::ParsePrimDec() {
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    Expect(Token::Tag::LPAREN);

    auto args = ParseTypeFields();
    Expect(Token::Tag::RPAREN);

    auto ret = TypeIdPtr();
    if (CurrIs(Token::Tag::COLON)) {
        auto type_id = Expect(Token::Tag::ID);
        ret = MakeUnique<TypeId>(type_id->Value());
    }

    return MakeUnique<PrimDec>(std::move(name),
            std::move(args), std::move(ret));
}

ImportDecPtr Parser::ParseImportDec() {
    return MakeUnique<ImportDec>(Expect(Token::Tag::STR)->Value());
}

TypePtr Parser::ParseType() {
    auto curr = NextToken();
    if (curr == nullptr) {
        PANIC("current token is null")
    }

    switch (curr->Type()) {
        case Token::Tag::LBRACE:
            return ParseRecordDef();
        case Token::Tag::ARRAY:
            return ParseArrayDef();
        case Token::Tag::CLASS:
            return ParseClassTypeDef();
        case Token::Tag::ID:
            return ParseAliasType();
        default:
            PANIC("current token's type is not valid")
    }

    return nullptr; // dead code
}

TypeAliasPtr Parser::ParseAliasType() {
    auto type = MakeUnique<TypeId>(Expect(Token::Tag::ID)->Value());
    return MakeUnique<TypeAlias>(std::move(type));
}

RecordDefPtr Parser::ParseRecordDef() {
    auto records = ParseTypeFields();
    Expect(Token::Tag::RBRACE);
    return MakeUnique<RecordDef>(std::move(records));
}

ArrayDefPtr Parser::ParseArrayDef() {
    Expect(Token::Tag::OF);
    auto type = MakeUnique<TypeId>(Expect(Token::Tag::ID)->Value());
    return MakeUnique<ArrayDef>(std::move(type));
}

ClassTypeDefPtr Parser::ParseClassTypeDef() {
    auto parent = TypeIdPtr();
    if (Try(Token::Tag::EXTENDS)) {
        parent = MakeUnique<TypeId>(Expect(Token::Tag::ID)->Value());
    }

    Expect(Token::Tag::LBRACE);
    auto fields = ParseClassFields();
    Expect(Token::Tag::RBRACE);

    return MakeUnique<ClassTypeDef>(std::move(parent), std::move(fields));
}

TypeFieldsPtr Parser::ParseTypeFields() {
    auto names = IdPtrVec();
    auto types = TypeIdPtrVec();

    if (CurrToken() == nullptr || !CurrIs(Token::Tag::ID)) {
        return nullptr;
    }
    do {
        auto id = Expect(Token::Tag::ID);
        Expect(Token::Tag::COLON);
        auto type_id = Expect(Token::Tag::ID);

        names.emplace_back(new Identifier(id->Value()));
        types.emplace_back(new TypeId(type_id->Value()));

    } while (Try(Token::Tag::COMMENT));

    return MakeUnique<TypeFields>(std::move(names), std::move(types));
}

ExprPtr Parser::ParseTopExpr() {
    auto left = ParsePrimeExpr();
    auto ops = OpPtrVec();
    auto rights = ExprPtrVec();
    auto curr = CurrToken();

    while ((curr = CurrToken()) != nullptr) {
        if (!curr->IsOperator()) {
            PANIC("expect operator in expr")
        }
        NextToken();
        auto rhs = ParseTopExpr();
        if (rhs == nullptr) {
            PANIC("right expr can't be null");
        }
        auto op_ptr = MakeUnique<Operator>(curr->Value());
        ops.push_back(std::move(op_ptr));
        rights.push_back(std::move(rhs));
    }
    return MakeUnique<Expr>(std::move(left),
            std::move(ops), std::move(rights));
}

PrimeExprPtr Parser::ParsePrimeExpr() {
    auto curr = CurrToken();
    if (curr == nullptr) {
        PANIC("expression can't be empty")
    }
    switch (curr->Type()) {
        case Token::Tag::NIL:
            return ParseNilExpr();
        case Token::Tag::NUM:
            return ParseIntExpr();
        case Token::Tag::STR:
            return ParseStrExpr();
        case Token::Tag::NEW:
            return ParseObjCrt();
        case Token::Tag::MINUS:
            return ParseUnaryExpr();
        case Token::Tag::LPAREN:
            return ParseExprs();
        case Token::Tag::IF:
            return ParseIf();
        case Token::Tag::WHILE:
            return ParseWhile();
        case Token::Tag::FOR:
            return ParseFor();
        case Token::Tag::BREAK:
            return ParseBreak();
        case Token::Tag::LET:
            return ParseLet();
        case Token::Tag::ID: {
            auto next = PeekNext();
            switch (next->Type()) {
                case Token::Tag::LBRACE:
                    return ParseRecordCrt();
                case Token::Tag::LSQUB:
                    return ParseArrayCrt();
                case Token::Tag::LPAREN:
                    return ParseFnCall();
                case Token::Tag::DOT:
                    return ParseVarExpr();
                default: {
                    return ParseVarExpr();
                }
            }
        }
        default:
            PANIC("invalid token")
    }
    return PrimeExprPtr();
}

NilExprPtr Parser::ParseNilExpr() {
    Expect(Token::Tag::NIL);
    return MakeUnique<NilExpr>();
}

IntExprPtr Parser::ParseIntExpr() {
    auto t = Expect(Token::Tag::NUM);
    auto num = std::stoi(t->Value());
    return MakeUnique<IntExpr>(num);
}

StrExprPtr Parser::ParseStrExpr() {
    auto t = Expect(Token::Tag::STR);
    return MakeUnique<StrExpr>(t->Value());
}

ArrayCreatePtr Parser::ParseArrayCrt() {
    auto type_id = Expect(Token::Tag::ID);
    auto type = MakeUnique<TypeId>(type_id->Value());
    auto lsqb = Expect(Token::Tag::LSQUB);
    auto size = ParseTopExpr();
    auto rsqb = Expect(Token::Tag::RSQUB);
    auto of = Expect(Token::Tag::OF);
    auto init = ParseTopExpr();

    return MakeUnique<ArrayCreate>(std::move(type),
            std::move(size), std::move(init));
}

RecordCreatePtr Parser::ParseRecordCrt() {
    auto type_id = Expect(Token::Tag::ID);
    auto type = MakeUnique<TypeId>(type_id->Value());
    auto field_names = TypeIdPtrVec();
    auto field_vars = ExprPtrVec();

    Expect(Token::Tag::LBRACE);
    if (CurrIs(Token::Tag::ID)) {
        do {
            auto id = CurrToken();
            auto name = MakeUnique<TypeId>(id->Value());
            auto _ = Expect(Token::Tag::EQ);
            auto exp = ParseTopExpr();

            field_names.push_back(std::move(name));
            field_vars.push_back(std::move(exp));

        } while (Try(Token::Tag::COMMA));
    }
    Expect(Token::Tag::RBRACE);

    return MakeUnique<RecordCreate>(std::move(type),
            std::move(field_names), std::move(field_vars));
}

ObjCreatePtr Parser::ParseObjCrt() {
    Expect(Token::Tag::NEW);
    auto type_id = Expect(Token::Tag::ID);
    auto type = MakeUnique<TypeId>(type_id->Value());
    return MakeUnique<ObjCreate>(std::move(type));
}

VarPtr Parser::ParseVar() {
    auto id = Expect(Token::Tag::ID);
    auto lhs = MakeUnique<Identifier>(id->Value());
    auto rhs = ParseVarA();
    return MakeUnique<Var>(std::move(lhs), std::move(rhs));
}

BasicVarPtr Parser::ParseBasicVar() {
    return MakeUnique<BasicVar>();
}

ArrayElemVarPtr Parser::ParseArrayElem() {
    Expect(Token::Tag::LSQUB);
    auto index = ParseTopExpr();
    Expect(Token::Tag::RSQUB);
    auto rhs = ParseVarA();
    return MakeUnique<ArrayElemVar>(std::move(rhs), std::move(index));
}

FieldVarPtr Parser::ParseFieldVar() {
    Expect(Token::Tag::DOT);
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    auto rhs = ParseVarA();
    return MakeUnique<FieldVar>(std::move(rhs), std::move(name));
}

FnCallPtr Parser::ParseFnCall() {
    auto id = Expect(Token::Tag::ID);
    auto fn_name = MakeUnique<Identifier>(id->Value());
    Expect(Token::Tag::LPAREN);
    auto args = ExprPtrVec();

   if (!CurrIs(Token::Tag::RPAREN)) {
       do {
           args.push_back(ParseTopExpr());
       } while (Try(Token::Tag::COMMA));
   }
   Expect(Token::Tag::RPAREN);

   return MakeUnique<FnCall>(std::move(fn_name), std::move(args));
}

// TODO
MethodCallPtr Parser::ParseMethodCall() {
    return MethodCallPtr();
}

// TODO
OpExprPtr Parser::ParseOpExpr() {
    return OpExprPtr();
}

AssignExprPtr Parser::ParseAssignExpr() {
    return AssignExprPtr();
}

IfExprPtr Parser::ParseIf() {
    Expect(Token::Tag::IF);
    auto cond = ParseTopExpr();
    Expect(Token::Tag::THEN);
    auto then = ParseTopExpr();
    auto else_ = ExprPtr();
    if (Try(Token::Tag::ELSE)) {
        else_ = ParseTopExpr();
    }
    return MakeUnique<IfExpr>(
            std::move(cond),
            std::move(then),
            std::move(else_));
}

WhileExprPtr Parser::ParseWhile() {
    Expect(Token::Tag::WHILE);
    auto cond = ParseTopExpr();
    Expect(Token::Tag::DO);
    auto body = ParseTopExpr();
    return MakeUnique<WhileExpr>(std::move(cond), std::move(body));
}

ForExprPtr Parser::ParseFor() {
    Expect(Token::Tag::FOR);
    auto name = MakeUnique<Identifier>(Expect(Token::Tag::ID)->Value());
    Expect(Token::Tag::ASSIGN);
    auto from = ParseTopExpr();
    Expect(Token::Tag::TO);
    auto to = ParseTopExpr();
    Expect(Token::Tag::DO);
    auto body = ParseTopExpr();
    return MakeUnique<ForExpr>(
            std::move(name),
            std::move(from),
            std::move(to),
            std::move(body));
}

BreakExprPtr Parser::ParseBreak() {
    Expect(Token::Tag::BREAK);
    return MakeUnique<BreakExpr>();
}

LetExprPtr Parser::ParseLet() {
    Expect(Token::Tag::LET);
    auto decs = ParseDecs();
    Expect(Token::Tag::IN);
    auto exps = ParseExprs();
    Expect(Token::Tag::END);
    return MakeUnique<LetExpr>(std::move(decs), std::move(exps));
}

ExprsExprPtr Parser::ParseExprs() {
    auto exps = ExprPtrVec();
    do {
        exps.push_back(ParseTopExpr());
    } while (Try(Token::Tag::COLON));
    return MakeUnique<ExprsExpr>(MakeUnique<Exprs>(std::move(exps)));
}

VarAPtr Parser::ParseVarA() {
    if (CurrToken() == nullptr) {
        return ParseBasicVar();
    }
    if (CurrIs(Token::Tag::DOT)) {
        return ParseFieldVar();
    }
    if (CurrIs(Token::Tag::LSQUB)) {
        return ParseArrayElem();
    }
    PANIC("no match var parse")
}

VarExprPtr Parser::ParseVarExpr() {
    return MakeUnique<VarExpr>(ParseVar());
}

UnaryExprPtr Parser::ParseUnaryExpr() {
    auto id = Expect(Token::Tag::MINUS);
    auto op = MakeUnique<Operator>(id->Value());
    auto expr = ParseTopExpr();
    return MakeUnique<UnaryExpr>(std::move(op), std::move(expr));
}
