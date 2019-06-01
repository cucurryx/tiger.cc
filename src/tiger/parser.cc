#include "parser.h"
#include "../utils/error.h"

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
    if (CurrIs(tag)) {
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
    return nullptr;
}
