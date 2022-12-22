function [learning_rate,n_iteration_needed]=chooseLearningRate(funcIn,a_min,a_max,a_step,boundaries,eps,stopRate, n_learner)
   learning_rate=a_min:a_step:a_max;
   n=size(learning_rate);
   n_iteration_needed=zeros(n);
   for i=1:max(n)
       [toto,I]=Learning(funcIn,boundaries,n_learner,eps,learning_rate(i),stopRate);
       n_iteration_needed(i)=I;
   end
end
