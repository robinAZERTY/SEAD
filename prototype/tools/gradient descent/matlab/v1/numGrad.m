function ret=numGrad(funcIn,varIn,eps)
n=length(varIn);
ret=zeros(n,1);
for i=1:1:n
    diffBy=zeros(n,1);
    diffBy(i)=1;
    ret(i)=numDiff(funcIn,varIn,diffBy,eps);
end
end