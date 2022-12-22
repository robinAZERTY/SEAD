function [ret,avr_iteration_number] = Learning(funcIn,boundaries,n_learner,eps,learningRate,stopRate)
min=1/0;
avr_iteration_number=0;
for i = 1:n_learner
    %create a random X between boundaries
    X=randVect(boundaries);
    [Y,Z]=gradDescent(funcIn,X,eps,learningRate,stopRate);
    avr_iteration_number=avr_iteration_number+Z;
    if min>funcIn(Y)
        ret=Y;
    end
end
avr_iteration_number=avr_iteration_number/n_learner;
end