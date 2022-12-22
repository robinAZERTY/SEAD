clc;
clf;

boundary=[-1 1  ; -1 1];
n_learner=100;
eps=1e-6;
learningRate=0.3;
stopRate=1e-6;

[ret,avr_iteration_number]=Learning(@hypersquared,boundary,n_learner,eps,learningRate,stopRate);
[learning_rate,n_iteration_needed]=chooseLearningRate(@hypersquared,0.1,0.8,0.003,boundary,eps,stopRate,n_learner);

figure(1),
plot(learning_rate,n_iteration_needed);
legend("N(alpha)");


boundary=[-2 2];

%[ret,avr_iteration_number]=Learning(@F,boundary,n_learner,eps,learningRate,stopRate)
[learning_rate,n_iteration_needed]=chooseLearningRate(@F,0.05,0.45,0.01,boundary,eps,stopRate,n_learner);

figure(2),
plot(learning_rate,n_iteration_needed);
legend("N(alpha)");

function ret=F(x)
ret=x*x*x*x - x*x + 0.1*x;
end

