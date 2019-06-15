# tiger.cc

A tiger compiler in modern C++. Both frontend and backend are written without automatic tools. The official grammer reference manual([https://www.lrde.epita.fr/~tiger/tiger.html](ttps://www.lrde.epita.fr/~tiger/tiger.html)) has provied all infomation about The Tiger Programming Langugae. However, the EBNF in that manual isn't LL(k), so we can't implement parser by this EBNF in a recursive decline way. Therefore, I transform it to LL(1) grammer([detail](./doc.md)).

## TODO
- [x] lexer
- [x] parser
- [ ] type checker
- [ ] ir generation
- [ ] backend  

## build & test
```
git clone https://github.com/xiebei1108/tiger.cc
cd tiger.cc
mkdir build && cd build
cmake .. && make
```

## detail

### AST
For example:
```
/* define a recursive function */
let

/* calculate n! */
function nfactor(n: int): int =
		if  n = 0 
			then 1
			else n * nfactor(n-1)

in
	nfactor(10)
end
```

it's AST is:

```
Expr(
 LetExpr(
  Decs(
    FnDec(
     Id(
      nfactor
     )
     TypeFields(
       Id(
        n
       )
       TypeId(
        int
       )
     )
     Expr(
      IfExpr(
       Expr(
        Lvar(
          Elem(
           Id(
            n
           )
          )
        )
         Op(
          =
         )
         Expr(
          IntExpr(
           0
          )
         )
       )
       Expr(
        IntExpr(
         1
        )
       )
       Expr(
        Lvar(
          Elem(
           Id(
            n
           )
          )
        )
         Op(
          *
         )
         Expr(
          FnCall(
           Id(
            nfactor
           )
            Expr(
             Lvar(
               Elem(
                Id(
                 n
                )
               )
             )
              Op(
               -
              )
              Expr(
               IntExpr(
                1
               )
              )
            )
          )
         )
       )
      )
     )
    )
  )
  Exprs(
    Expr(
     FnCall(
      Id(
       nfactor
      )
       Expr(
        IntExpr(
         10
        )
       )
     )
    )
  )
 )
)
```

## target

