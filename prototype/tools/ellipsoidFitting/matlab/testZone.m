clc;
clf;

boundaries=[-1 1  ; -1 1];
n_learner=5;
eps=1e-10;
n_iteration=30;

%[ret,J] = Learning(@hypersquared,boundaries,n_learner,eps,learningRate,n_iteration)

[learning_rate,J]=chooseLearningRate(@hypersquared,1e-3,2,1.025,boundaries,eps,n_iteration, n_learner);
figure(1);
loglog(learning_rate,J,".");
legend("error(alpha)");