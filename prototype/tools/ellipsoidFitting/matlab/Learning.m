function [ret,J] = Learning(funcIn,boundaries,n_learner,eps,learningRate,n_iteration)

J=Inf;

for i = 1:n_learner
    %create a random X between boundaries
    X=randVect(boundaries);
    Y=gradDescent(funcIn,X,eps,learningRate,n_iteration);
    e=funcIn(Y);
    if(e<J)
        J=e;
        ret=Y;
    end
end
end