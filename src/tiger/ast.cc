//
// Created by 解贝 on 2019-05-30.
//

#include "ast.h"

std::string Identifier::ToString(u32 depth) {
    return std::string(depth, '\t') + "Id(" + name_ + ")";
}

std::string Operator::ToString(u32 depth) {
    return std::string(depth, '\t') + "Op(" + op_ + ")";
}

std::string TypeId::ToString(u32 depth) {
    return std::string(depth, '\t') + "TypeId(" + name_ + ")";
}

std::string Expr::ToString(u32 depth) {
    auto str = std::string(depth, '\t') + "Expr(\n";
    str += left_->ToString(depth + 1) + "\n";
    for (auto &op : ops_) {
        str += op->ToString(depth + 1) + "\n";
    }
    for (auto &r: rights_) {
        str += r->ToString(depth + 1) + "\n";
    }
    str += std::string(depth, '\t') + ")";
    return str;
}

std::string PrimeExpr::ToString(u32 depth) {
    return std::string(depth, '\t') + "PrimExpr()";
}

std::string NilExpr::ToString(u32 depth) {
    return std::string(depth, '\t') + "NilExpr()";
}

std::string IntExpr::ToString(u32 depth) {
    return std::string(depth, '\t') + "IntExpr(" + std::to_string(num_) + ")";
}

std::string UnaryExpr::ToString(u32 depth) {
    return std::string(depth, '\t')
        + "UnaryExpr(\n"
        + op_->ToString(depth + 1)
        + "\n"
        + expr_->ToString(depth + 1)
        + "\n"
        + std::string(depth, '\t')
        + ")";
}

std::string StrExpr::ToString(u32 depth) {
    return std::string(depth, '\t') + "StrExpr(" + str_ + ")";
}

std::string ArrayCreate::ToString(u32 depth) {
    return std::string(depth, '\t')
        + "ArrayCreate(\n"
        + type_id_->ToString(depth + 1)
        + "\n"
        + len_->ToString(depth + 1)
        + "\n"
        + init_->ToString(depth + 1)
        + "\n"
        + std::string(depth, '\t')
        + ")";
}

std::string RecordCreate::ToString(u32 depth) {
    auto str = std::string(depth, '\t') + "RecordCreate(\n";
    str += type_id_->ToString(depth);
    for (auto &p : types_) {
        str += p->ToString(depth);
    }
    for (auto &p : vars_) {
        str += p->ToString(depth);
    }
    return str + ")";
}

std::string ObjCreate::ToString(u32 depth) {
    return "ObjCreate(" + type_id_->ToString(depth) + ")";
}

std::string Var::ToString(u32 depth) {
    return "Var(" + id_->ToString(depth) + rhs_->ToString(depth) + ")";
}

std::string VarA::ToString(u32 depth) {
    return "VarA()";
}

std::string BasicVar::ToString(u32 depth) {
    return "BasicVar()";
}

std::string FieldVar::ToString(u32 depth) {
    return "FieldVar(" + lvar_->ToString(depth) + name_->ToString(depth) + ")";
}

std::string ArrayElemVar::ToString(u32 depth) {
    return "ArrayElemVar(" + lvar_->ToString(depth) + index_->ToString(depth) + ")";
}

std::string VarExpr::ToString(u32 depth) {
    return "VarExpr(" + lvar_->ToString(depth) + ")";
}

std::string FnCall::ToString(u32 depth) {
    auto str = "FnCall(" + name_->ToString(depth);
    for (auto &arg : args_) {
        str += arg->ToString(depth);
    }
    return str + ")";
}

std::string MethodCall::ToString(u32 depth) {
    auto str = "MethodCall(" + lvar_->ToString(depth) + method_->ToString(depth);
    for (auto &arg : args_) {
        str += arg->ToString(depth);
    }
    return str + ")";
}

std::string OpExpr::ToString(u32 depth) {
    return "OpExpr(" + op_->ToString(depth) + lhs_->ToString(depth) + rhs_->ToString(depth) + ")";
}

std::string Exprs::ToString(u32 depth) {
    auto str = std::string("Exprs(");
    for (auto &e : exprs_) {
        str += e->ToString(depth);
    }
    return str + ")";
}

std::string ExprsExpr::ToString(u32 depth) {
    return "ExprsExpr(" + exprs_->ToString(depth) + ")";
}

std::string AssignExpr::ToString(u32 depth) {
    return "AssignExpr(" + lval_->ToString(depth) + expr_->ToString(depth) + ")";
}

std::string IfExpr::ToString(u32 depth) {
    return "IfExpr(" + if_->ToString(depth) + then_->ToString(depth) + else_->ToString(depth) + ")";
}

std::string WhileExpr::ToString(u32 depth) {
    return "WhileExpr(" + while_->ToString(depth) + do_->ToString(depth) + ")";
}

std::string ForExpr::ToString(u32 depth) {
    return "ForExpr(" + id_->ToString(depth) + from_->ToString(depth) 
        + to_->ToString(depth) + do_->ToString(depth) + ")";
}

std::string BreakExpr::ToString(u32 depth) {
    return "BreakExpr()";
}

std::string LetExpr::ToString(u32 depth) {
    return "LetExpr(" + decs_->ToString(depth) + exprs_->ToString(depth) + ")";
}

std::string Decs::ToString(u32 depth) {
    auto str = std::string(depth, '\t') + "Decs(\n";
    for (auto &p : decs_) {
        str += p->ToString(depth + 1) + "\n";
    }
    return str + std::string(depth, '\t') + ")";
}

std::string TypeDec::ToString(u32 depth) {
    return std::string(depth, '\t') 
        + "TypeDec(\n" 
        + name_->ToString(depth + 1) 
        + "\n"
        + type_->ToString(depth + 1) 
        + "\n"
        + std::string(depth, '\t') 
        + ")";
}

std::string ClassDef::ToString(u32 depth) {
    auto parent = std::string();
    if (parent_ != nullptr) {
        parent = parent_->ToString(depth);
    }
    return "ClassDef(" + name_->ToString(depth) + parent + fields_->ToString(depth) + ")";
}

std::string VarDec::ToString(u32 depth) {
    return "VarDec(" + name_->ToString(depth) + type_->ToString(depth) + var_->ToString(depth) + ")";
}

std::string FnDec::ToString(u32 depth) {
    return "FnDec(" + name_->ToString(depth) + args_->ToString(depth) + body_->ToString(depth) + ")";
}

std::string PrimDec::ToString(u32 depth) {
    return "PrimDec(" + name_->ToString(depth) + args_->ToString(depth) + ret_->ToString(depth) + ")";
}

std::string ImportDec::ToString(u32 depth) {
    return std::string(depth, '\t') + "ImportDec(" + import_ + ")";
}

std::string ClassField::ToString(u32 depth) {
    return "ClassField()";
}

std::string ClassFields::ToString(u32 depth) {
    auto str = std::string("ClassFields(");
    for (auto &p : fields_) {
        str += p->ToString(depth);
    }
    return str + ")";
}

std::string AttrDec::ToString(u32 depth) {
    return "AttrDec(" + attr_->ToString(depth) + ")";
}

std::string MethodDec::ToString(u32 depth) {
    return "MethodDec(" + name_->ToString(depth) + args_->ToString(depth) 
        + ret_->ToString(depth) + body_->ToString(depth) + ")";
}

std::string Type::ToString(u32 depth) {
    return "Type()";
}

std::string TypeAlias::ToString(u32 depth) {
    return "TypeAlias(" + alias_->ToString(depth) + ")";
}

std::string RecordDef::ToString(u32 depth) {
    return std::string(depth, '\t')  
        + "RecordDef(\n" 
        + records_->ToString(depth + 1) 
        + "\n"
        + std::string(depth, '\t')
        + ")";
}

std::string ArrayDef::ToString(u32 depth) {
    return std::string(depth, '\t')  
        + "ArrayDef(\n" 
        + type_->ToString(depth)
        + "\n"
        + std::string(depth, '\t')  
        + ")";
}

std::string ClassTypeDef::ToString(u32 depth) {
    return "ClassTypeDef(" + parent_->ToString(depth) + fields_->ToString(depth) + ")";
}

std::string TypeFields::ToString(u32 depth) {
    auto str = std::string(depth, '\t')  + "TypeFields(\n";
    for (auto &p : names_) {
        str += p->ToString(depth + 1) + "\n";
    }
    for (auto &p : types_) {
        str += p->ToString(depth + 1) + "\n";
    }
    return str + std::string(depth, '\t')  + ")";
}