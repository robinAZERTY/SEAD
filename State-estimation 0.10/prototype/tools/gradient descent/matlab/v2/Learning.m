function [ret,J] = Learning(funcIn,boundaries,n_learner,eps,learningRate,n_iteration)
for i = 1:n_learner
    %create a random X between boundaries
    X=randVect(boundaries);
    Y=gradDescent(funcIn,X,eps,learningRate,n_iteration);
end
ret=Y;
J=funcIn(Y);

end