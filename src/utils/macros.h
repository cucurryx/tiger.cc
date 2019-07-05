#ifndef TIGER_CC_MACROS_H
#define TIGER_CC_MACROS_H

#define DEFINE_PTR(x) using x##Ptr = std::shared_ptr<x>;
#define DEFINE_VEC(x) using x##Vec = std::vector<x>;

#define ACCEPTOR virtual void Accept(VisitorPtr visitor) { visitor->Visit(this); }

#endif