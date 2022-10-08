function ret=numDiff(funcIn,varIn,diffBy,eps)
x1 = varIn-diffBy.*(eps/2);
x2 = varIn+diffBy.*(eps/2);
f1 = funcIn(x1);
f2 = funcIn(x2);
ret = (f2-f1)/eps;
end