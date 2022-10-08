function ret = gradDescent(funcIn,varInit,eps,learningRate,n_iteration)
ret=varInit;
for i=1:n_iteration
    grad=numGrad(funcIn,ret,eps);
    ret=ret-grad.*learningRate;
end
end