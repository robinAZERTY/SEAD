function [ret,J] = Learning2(funcIn,boundaries,n_learner,eps,learningRate,n_iteration)
ret=zeros([length(boundaries) n_iteration+1]);
J=ones([1 n_iteration]).*Inf;

for i = 1:n_learner
    %create a random X between boundaries
    X=randVect(boundaries);
    Y=gradDescent2(funcIn,X,eps,learningRate,n_iteration);
    e=funcIn(Y(:,n_iteration+1));
    if(e<J(n_iteration))
        for j= 1:n_iteration
            J(j)=funcIn(Y(:,j));
        end
        
        ret(:,:)=Y(:,:);
    end
end
end