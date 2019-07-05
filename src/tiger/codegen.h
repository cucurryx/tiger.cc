#ifndef TIGER_CC_CODEGEN_H
#define TIGER_CC_CODEGEN_H

#include "visitor.h"

class CodeGen: public Visitor {
public:

    void Visit(IntExprPtr node);
    void Visit(StrExprPtr node);
    void Visit(NilExprPtr node);
    void Visit(ExprSeqPtr node);
    void Visit(AssignmentPtr node);
    void Visit(UnaryExprPtr node);
    void Visit(ExprsPtr node);
    void Visit(BinaryExprPtr node);
    void Visit(ArrayCreatePtr node);
    void Visit(RecordCreatePtr node);
    void Visit(ObjectNewPtr node);
    void Visit(MethodCallPtr node);
    void Visit(FnCallPtr node);
    void Visit(IfStmtPtr node);
    void Visit(WhileStmtPtr node);
    void Visit(ForStmtPtr node);
    void Visit(BreakStmtPtr node);
    void Visit(LetStmtPtr node);
    void Visit(ElemPtr node);
    void Visit(LvarPtr node);
    void Visit(ClassFieldPtr node);
    void Visit(ClassFieldsPtr node);
    void Visit(TypeFieldsPtr node);
    void Visit(DecPtr node);
    void Visit(VarDecPtr node);
    void Visit(DecsPtr node);
    void Visit(MethodDecPtr node);
    void Visit(AttrDecPtr node);
    void Visit(FnDecPtr node);
    void Visit(PrimDecPtr node);
    void Visit(ImportDecPtr node);
    void Visit(TypeAliasPtr node);
    void Visit(RecordDefPtr node);
    void Visit(ArrayDefPtr node);
    void Visit(ClassDefPtr node);
    void Visit(ClassTypeDefPtr node);

private:

};

#endif // TIGER_CC_CODEGEN_H