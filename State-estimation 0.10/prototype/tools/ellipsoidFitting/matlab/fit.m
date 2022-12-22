clc;

n=3000; %nombre d'échantillons
global noise;
global P_real;
global P_samples;
global boundaries;
global iteration;
global P_correct;
global n_learner;
global alpha;


noise=0.01;
[x,y,z]=cloud_sphere(n,noise);
P_real=transpose([x y z]);
radius=1;
maxGainNoise=0.6;
maxNonOrtho=0.4;
maxOffset=5;
iteration=300;
alpha=0.1;
n_learner=1;
[A,B]=ellipse_rand(radius,maxGainNoise,maxNonOrtho,maxOffset)

P_samples=A*P_real+B;
center = [average(P_samples(1,:));average(P_samples(2,:));average(P_samples(3,:))];
approxGain=[max(P_samples(1,:))-min(P_samples(1,:));max(P_samples(2,:))-min(P_samples(2,:));max(P_samples(3,:))-min(P_samples(3,:))]./2;


boundaries=[approxGain(1,1)-maxGainNoise approxGain(1,1)+maxGainNoise;-maxNonOrtho maxNonOrtho;-maxNonOrtho maxNonOrtho;-maxNonOrtho maxNonOrtho;approxGain(1,1)-maxGainNoise approxGain(1,1)+maxGainNoise;-maxNonOrtho maxNonOrtho;-maxNonOrtho maxNonOrtho;-maxNonOrtho maxNonOrtho;approxGain(1,1)-maxGainNoise approxGain(1,1)+maxGainNoise;center(1) center(1);center(2) center(2);center(3) center(3)];
%boundaries=[approxGain(1,1) approxGain(1,1);0 0;0 0;0 0;approxGain(2,1) approxGain(2,1);0 0;0 0;0 0;approxGain(3,1) approxGain(3,1);center(1) center(1);center(2) center(2);center(3) center(3)];
clf;


%findAlpha(1e-3,1e3,1.3,iteration,n_learner);
showw();


function ret=cost(parameters)
global P_samples;
global noise;
P_corrected=correct(P_samples,parameters);
avr=0;
n=length(P_corrected);
    for i=1:n
        avr=avr+ep(abs(norm(P_corrected(:,i))-1));
    end
    [A,B]=get_AB(parameters);
    [~,~,nonOrtho,~] = explicit(A,B);
    rotationcost=(ep(dot(nonOrtho(1,:),[1 0 0])-1)+ep(dot(nonOrtho(2,:),[0 1 0])-1)+ep(dot(nonOrtho(3,:),[0 0 1])-1))/3;
    
ret=avr/n;
ret=ret+rotationcost;
end

function ret = ep(x)
%ret=exp(-2*x)/2+x-1/2;
ret=x^2;
end
function P_corrected=correct(samples,parameters)
[AA,BB]=get_AB(parameters);
P_corrected=AA\(samples-BB);
end

function parameters=get_parameters(AA,BB)
    parameters=zeros([1 12]);
    parameters(1)=AA(1,1);
    parameters(2)=AA(1,2);
    parameters(3)=AA(1,3);
    parameters(4)=AA(2,1);
    parameters(5)=AA(2,2);
    parameters(6)=AA(2,3);
    parameters(7)=AA(3,1);
    parameters(8)=AA(3,2);
    parameters(9)=AA(3,3);
    parameters(10)=BB(1,1);
    parameters(11)=BB(2,1);
    parameters(12)=BB(3,1);
end

function [AA,BB]=get_AB(parameters)
AA=zeros([3 3]);
BB=zeros([3 1]);
AA(1,1)=parameters(1);
AA(1,2)=parameters(2);
AA(1,3)=parameters(3);
AA(2,1)=parameters(4);
AA(2,2)=parameters(5);
AA(2,3)=parameters(6);
AA(3,1)=parameters(7);
AA(3,2)=parameters(8);
AA(3,3)=parameters(9);
BB(1,1)=parameters(10);
BB(2,1)=parameters(11);
BB(3,1)=parameters(12);
end

function [x,y,z]=cloud_sphere(n,noise)%generation de points bruités distribuées homogènement sur une sphere unitaire
rvals = 2*(rand(n,1)-0.5);
elevation = asin(rvals);
azimuth = 2*pi*rand(n,1);
radius= randn(n,1).*noise+1;
[x,y,z] = sph2cart(azimuth,elevation,radius);
end


function findAlpha(a_min,a_max,a_factor,iteration,n_learner)
global boundaries;
eps=1e-10;
[learning_rate,J]=chooseLearningRate(@cost,a_min,a_max,a_factor,boundaries,eps,iteration, n_learner);
J=J.^0.5;
figure(2);
loglog(learning_rate,J,".");
legend("error(alpha)");
end


function showw()
global noise;
global P_real;
global P_samples;
global boundaries;
global iteration;
global P_correct;
global n_learner;
global alpha;

[params,I]=Learning2(@cost,boundaries,n_learner,1e-6,alpha,iteration);

figure(1);
hold on;
view(3);

%plot3(P_samples(1,:),P_samples(2,:),P_samples(3,:),".");
p=plot3(P_correct(1,:),P_correct(2,:),P_correct(3,:),".");
plot3(P_real(1,:),P_real(2,:),P_real(3,:),".");
axis equal;
legend("Calibrated","Perfect");
clearpoints;
refreshdata;
p.XDataSource = 'P_correct(1,:)';
p.YDataSource = 'P_correct(2,:)';
p.ZDataSource = 'P_correct(3,:)';

for t = 1:iteration
    P_correct=correct(P_samples,params(:,t));
    legend("Calibrated at i = "+string(t),"Perfect");
    refreshdata
    drawnow
    %pause(1/t);
end

params=params(:,length(params)-1);
[A_est,B_est]=get_AB(params)
[radius,gains,nonOrtho,offsets]=explicit(A_est,B_est)
Error=I^0.5
end
