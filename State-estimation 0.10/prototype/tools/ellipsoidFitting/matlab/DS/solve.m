syms a b c x y

eq=a/(((x-b)/c)^2+1)==y

eqq=expand(eq)
A= children(collect(lhs(eqq),x))
xx_=combine(expand(A{1}/x^2))
x_=combine(expand(A{2}/x))

%syms x1 y1 x2 y2 x3 y3


%eq1 = subs(eq,[x y],[x1 y1])
%eq2 = subs(eq,[x y],[x2 y2])
%eq3 = subs(eq,[x y],[x3 y3])

%system=[eq1 eq2 eq3]

%solve(system,[a b c])