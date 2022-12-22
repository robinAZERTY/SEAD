function [learning_rate,J]=chooseLearningRate(funcIn,a_min,a_max,a_step,boundaries,eps,n_iteration, n_learner)
   n=floor(logb(a_step,a_max/a_min));
   learning_rate=zeros([1,n]);
   J=zeros(n);
   for i=1:n
       clc;
       loading=string(i)+"\"+string(n)
       learning_rate(i)=a_min*a_step^i;
       [~,I]=Learning(funcIn,boundaries,n_learner,eps,learning_rate(i),n_iteration);
       J(i)=I;
   end
end

function ret = logb(base,x)
ret= log(x)/log(base);
end