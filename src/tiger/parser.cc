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
    return ParseMain();
}

Lexer::TokenPtrVec Parser::FilterTokens(Lexer::TokenPtrVec tokens) {
    auto result = Lexer::TokenPtrVec();
    for (auto &p : tokens) {
        if (p == nullptr
            || p->Type() == Token::Tag::COMMENT
            || p->Type() == Token::Tag::EOL) {
            continue;
        } else {
            result.push_back(p);
        }
    }
    return result;
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
    if (auto curr_tag = CurrToken()->Type(); curr_tag != tag) {
        auto msg = std::string("[Parser::Expect] not expected token's tag. ")
                +  "expect " + Token::TagStr(tag)
                +  ". but we get " + Token::TagStr(curr_tag);
        PANIC(msg.c_str())
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
            break;
        default:
            result = ParseTopExpr();
    }

    return result;
}

DecsPtr Parser::ParseDecs() {
    auto decs = DecPtrVec();
    auto curr = CurrToken();
    while ((curr = CurrToken()) != nullptr) {
        switch (curr->Type()) {
            case Token::Tag::TYPE:
            case Token::Tag::CLASS:
            case Token::Tag::FUNCTION:
            case Token::Tag::PRIMITIVE:
            case Token::Tag::IMPORT:
            case Token::Tag::VAR:
                decs.push_back(std::move(ParseDec()));
                break;
            default:
                return MakeUnique<Decs>(std::move(decs));
        }
    }
    return MakeUnique<Decs>(std::move(decs));
}

DecPtr Parser::ParseDec() {
    auto dec = DecPtr();
    auto token = CurrToken();
    if (token == nullptr) {
        return dec;
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
            break;
    }
    return dec;
}

DecPtr Parser::ParseTypeDec() {
    Expect(Token::Tag::TYPE);
    auto id = Expect(Token::Tag::ID);
    auto _ = Expect(Token::Tag::EQ);
    auto type = ParseType();
    auto name = MakeUnique<Identifier>(id->Value());
    return MakeUnique<TypeDec>(std::move(name), std::move(type));
}

DecPtr Parser::ParseClassDefA() {
    Expect(Token::Tag::CLASS);
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
    auto curr = CurrToken();
    while ((curr = CurrToken()) != nullptr) {
        switch (curr->Type()) {
            case Token::Tag::VAR:
            case Token::Tag::METHOD:
                fields.push_back(ParseClassField());
                break;
            default:
                return MakeUnique<ClassFields>(std::move(fields));
        }
    }
    return MakeUnique<ClassFields>(std::move(fields));
}

// class-field ::= attr-dec | method-dec
ClassFieldPtr Parser::ParseClassField() {
    if (CurrIs(Token::Tag::VAR)) {
        return ParseAttrDec();
    }
    if (CurrIs(Token::Tag::METHOD)) {
        return ParseMethodDec();
    }
    PANIC("class field expect `var` or `method`\n")
    return nullptr;
}

// method declaration in class fields
// method id (type-fields) [ : type-id ] = exp
MethodDecPtr Parser::ParseMethodDec() {
    Expect(Token::Tag::METHOD);
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
    Expect(Token::Tag::VAR);
    auto id = Expect(Token::Tag::ID);
    auto name = MakeUnique<Identifier>(id->Value());
    auto type = TypeIdPtr();

    if (Try(Token::Tag::COLON)) {
        auto type_id = Expect(Token::Tag::ID);
        type = MakeUnique<TypeId>(type_id->Value());
    }

    Expect(Token::Tag::ASSIGN);
    auto body = ParseTopExpr();
    return MakeUnique<VarDec>(
            std::move(name), std::move(type), std::move(body));
}

FnDecPtr Parser::ParseFnDec() {
    Expect(Token::Tag::FUNCTION);
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
    return MakeUnique<FnDec>(std::move(name), std::move(args),
            std::move(ret), std::move(body));
}

PrimDecPtr Parser::ParsePrimDec() {
    Expect(Token::Tag::PRIMITIVE);
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
    Expect(Token::Tag::IMPORT);
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
            return MakeUnique<TypeAlias>(MakeUnique<TypeId>(curr->Value()));
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
        return MakeUnique<TypeFields>(std::move(names), std::move(types));
    }
    
    do {
        auto id = Expect(Token::Tag::ID);
        Expect(Token::Tag::COLON);
        auto type_id = Expect(Token::Tag::ID);

        names.emplace_back(new Identifier(id->Value()));
        types.emplace_back(new TypeId(type_id->Value()));

    } while (Try(Token::Tag::COMMA));

    return MakeUnique<TypeFields>(std::move(names), std::move(types));
}

ExprPtr Parser::ParseTopExpr() {
    auto left = ParsePrimeExpr();
    auto ops = OperatorPtrVec();
    auto rights = ExprPtrVec();
    auto curr = CurrToken();

    while ((curr = CurrToken()) != nullptr && curr->IsOperator()) {
        if (!curr->IsOperator()) {
            auto msg = std::string("expect operator in expr, curr is: " + curr->Name() + "(" + curr->Value() + ")");
            PANIC(msg.c_str())
        }
        auto op_ptr = MakeUnique<Operator>(curr->Value());
        NextToken();
        auto rhs = ParseTopExpr();
        if (rhs == nullptr) {
            PANIC("right expr can't be null");
        }
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
            return ParseObjectNew();
        case Token::Tag::MINUS:
            return ParseUnaryExpr();
        case Token::Tag::LPAREN:
            return ParseExprSeq();
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
                case Token::Tag::LPAREN:
                    return ParseFnCall();
                default:
                    return ParseExprTail();
            }
        }
        default:
            PANIC("invalid token")
    }
    return PrimeExprPtr();
}

PrimeExprPtr Parser::ParseExprTail() {
    auto elem = ElemPtr();

    if (PeekNext()->Type() == Token::Tag::LSQUB) {
        auto id = NextToken();
        Expect(Token::Tag::LSQUB);
        auto len = ParseTopExpr();
        Expect(Token::Tag::RSQUB);

        if (Try(Token::Tag::OF)) {
            // array creation
            auto init = ParseTopExpr();
            auto type = MakeUnique<TypeId>(id->Value());
            return MakeUnique<ArrayCreate>(std::move(type), std::move(len), std::move(init));
        }

        auto idxs = ExprPtrVec();
        idxs.push_back(std::move(len));
        while (Try(Token::Tag::LSQUB)) {
            auto idx = ParseTopExpr();
            Expect(Token::Tag::RSQUB);
            idxs.push_back(std::move(idx));
        }

        elem = MakeUnique<Elem>(MakeUnique<Identifier>(id->Value()), std::move(idxs));
    } else {
        auto id = NextToken();
        elem = MakeUnique<Elem>(MakeUnique<Identifier>(id->Value()), ExprPtrVec());
    }

    // lvar
    auto lvar = ParseLvar(std::move(elem));

    if (Try(Token::Tag::DOT)) {
        auto id = Expect(Token::Tag::ID);
        auto method = MakeUnique<Identifier>(id->Value());
        Expect(Token::Tag::LPAREN);

        auto args = ExprPtrVec();
        while (!CurrIs(Token::Tag::RPAREN)) {
            args.push_back(ParseTopExpr());
            if (!Try(Token::Tag::COMMA)) {
                break;
            }
        }
        Expect(Token::Tag::RPAREN);
        return MakeUnique<MethodCall>(std::move(lvar), std::move(method), std::move(args));

    } else if (Try(Token::Tag::ASSIGN)) {
        // assignment
        auto rvar = ParseTopExpr();
        return MakeUnique<Assignment>(std::move(lvar), std::move(rvar));

    } else {
        // lvalue
        return std::move(lvar);
    }
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

RecordCreatePtr Parser::ParseRecordCrt() {
    auto type_id = Expect(Token::Tag::ID);
    auto type = MakeUnique<TypeId>(type_id->Value());
    auto field_names = TypeIdPtrVec();
    auto field_vars = ExprPtrVec();

    Expect(Token::Tag::LBRACE);
    if (CurrIs(Token::Tag::ID)) {
        do {
            auto id = Expect(Token::Tag::ID);
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

ObjectNewPtr Parser::ParseObjectNew() {
    Expect(Token::Tag::NEW);
    auto type_id = Expect(Token::Tag::ID);
    auto type = MakeUnique<TypeId>(type_id->Value());
    return MakeUnique<ObjectNew>(std::move(type));
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

IfStmtPtr Parser::ParseIf() {
    Expect(Token::Tag::IF);
    auto cond = ParseTopExpr();
    Expect(Token::Tag::THEN);
    auto then = ParseTopExpr();
    auto else_ = ExprPtr();
    if (Try(Token::Tag::ELSE)) {
        else_ = ParseTopExpr();
    }
    return MakeUnique<IfStmt>(
            std::move(cond),
            std::move(then),
            std::move(else_));
}

WhileStmtPtr Parser::ParseWhile() {
    Expect(Token::Tag::WHILE);
    auto cond = ParseTopExpr();
    Expect(Token::Tag::DO);
    auto body = ParseTopExpr();
    return MakeUnique<WhileStmt>(std::move(cond), std::move(body));
}

ForStmtPtr Parser::ParseFor() {
    Expect(Token::Tag::FOR);
    auto name = MakeUnique<Identifier>(Expect(Token::Tag::ID)->Value());
    Expect(Token::Tag::ASSIGN);
    auto from = ParseTopExpr();
    Expect(Token::Tag::TO);
    auto to = ParseTopExpr();
    Expect(Token::Tag::DO);
    auto body = ParseTopExpr();
    return MakeUnique<ForStmt>(
            std::move(name),
            std::move(from),
            std::move(to),
            std::move(body));
}

BreakStmtPtr Parser::ParseBreak() {
    Expect(Token::Tag::BREAK);
    return MakeUnique<BreakStmt>();
}

LetStmtPtr Parser::ParseLet() {
    Expect(Token::Tag::LET);
    auto decs = ParseDecs();
    Expect(Token::Tag::IN);
    auto exps = ParseExprs();
    Expect(Token::Tag::END);
    return MakeUnique<LetStmt>(std::move(decs), std::move(exps));
}

ExprSeqPtr Parser::ParseExprSeq() {
    Expect(Token::Tag::LPAREN);
    auto exps = ParseExprs();
    Expect(Token::Tag::RPAREN);
    return MakeUnique<ExprSeq>(std::move(exps));
}

ExprsPtr Parser::ParseExprs() {
    auto exps = ExprPtrVec();
    if (auto curr = CurrToken(); curr != nullptr) {
        switch (curr->Type()) {
            case Token::Tag::NIL:
            case Token::Tag::NUM:
            case Token::Tag::STR:
            case Token::Tag::NEW:
            case Token::Tag::MINUS:
            case Token::Tag::LPAREN:
            case Token::Tag::IF:
            case Token::Tag::WHILE:
            case Token::Tag::FOR:
            case Token::Tag::BREAK:
            case Token::Tag::LET:
            case Token::Tag::ID:
                do {
                    exps.push_back(ParseTopExpr());
                } while (Try(Token::Tag::SEMI));
            default: break;
        }
    }
    return MakeUnique<Exprs>(std::move(exps));
}

UnaryExprPtr Parser::ParseUnaryExpr() {
    auto id = Expect(Token::Tag::MINUS);
    auto op = MakeUnique<Operator>(id->Value());
    auto expr = ParseTopExpr();
    return MakeUnique<UnaryExpr>(std::move(op), std::move(expr));
}

ElemPtr Parser::ParseElem() {
    auto id = Expect(Token::Tag::ID);
    auto idxs = ExprPtrVec();
    while (Try(Token::Tag::LSQUB)) {
        idxs.push_back(ParseTopExpr());
        Expect(Token::Tag::RSQUB);
    }
    return MakeUnique<Elem>(MakeUnique<Identifier>(id->Value()), std::move(idxs));
}

LvarPtr Parser::ParseLvar(ElemPtr elem) {
    auto elems = ElemPtrVec();
    elems.push_back(std::move(elem));
    while (Try(Token::Tag::DOT)) {
        // method call
        if (PeekNext()->Type() == Token::Tag::LPAREN) {
            break;
        }
        elems.push_back(ParseElem());
    }
    return MakeUnique<Lvar>(std::move(elems));
}
