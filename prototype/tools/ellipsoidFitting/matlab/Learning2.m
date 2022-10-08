function [ret,J] = Learning2(funcIn,boundaries,n_learner,eps,learningRate,n_iteration)
ret=zeros([length(boundaries) n_iteration+1]);
J=Inf;

for i = 1:n_learner
    %create a random X between boundaries
    X=randVect(boundaries);
    Y=gradDescent2(funcIn,X,eps,learningRate,n_iteration);
    e=funcIn(Y(:,n_iteration+1));
    if(e<J)
        J=e;
        ret(:,:)=Y(:,:);
    end
end
end