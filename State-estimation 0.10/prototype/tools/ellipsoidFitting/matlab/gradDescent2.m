function ret = gradDescent2(funcIn,varInit,eps,learningRate,n_iteration)
ret=zeros([length(varInit) n_iteration+1]);
ret(:,1)=varInit;

for i=1:n_iteration
    grad=numGrad(funcIn,ret(:,i),eps);
    ret(:,i+1)=ret(:,i)-grad.*learningRate;
end
end