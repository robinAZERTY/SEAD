%##############################
%####Variables globales########
%##############################
global noise;
global P_real;
global P_samples;
global boundaries;
global iteration;
global P_correct;
global n_learner;
global alpha;

global radius1;

%##############################
%##Parametres de simulation####
%##############################

%##generation d'échantiollon###
n=200; %nombre d'échantillons
noise=0.05;%bruit relatif (sur la sphere unitaire)
radius=43.56;
maxGainNoise=0.35;
maxNonOrtho=0.25;
maxOffset=radius*1.5;


%##parametres d'apprentissage##
iteration=15;
alpha=0.7;
n_learner=1;


%##############################
%######début du programme######
%##############################

clc;
clf;

%points generation
[x,y,z]=cloud_sphere(n,noise);
P_real=transpose([x y z]);%in the unit sphere

%create random calibration data (an ellepsoid)
[A,B]=ellipse_rand(radius,maxGainNoise,maxNonOrtho,maxOffset)
[radius,gains,nonOrtho,offsets] = explicit(A,B);
min_rot_cost=rotation_cost(get_parameters(A,B));
P_samples=A*P_real+B;
transpose(P_samples)
[radius1,gains1,offsets1]=firstApprox(P_samples);

%first approximation
center = [average(P_samples(1,:));average(P_samples(2,:));average(P_samples(3,:))];
approxGain=[max(P_samples(1,:))-min(P_samples(1,:));max(P_samples(2,:))-min(P_samples(2,:));max(P_samples(3,:))-min(P_samples(3,:))]./2;

%fixed and defined by pre-calibration
boundaries=[approxGain(1,1) approxGain(1,1);0 0;0 0;0 0;approxGain(2,1) approxGain(2,1);0 0;0 0;0 0;approxGain(3,1) approxGain(3,1);center(1) center(1);center(2) center(2);center(3) center(3)];

%fitting to the unit sphere
[params,I]=Learning2(@cost,boundaries,n_learner,1e-6,alpha,iteration);
[AA,BB]=get_AB(params(:,iteration));

%pre_calibration to the unit sphere
pre_calibration=correct(P_samples,params(:,1));

%correct to the unit sphere
P_correctU=correct(P_samples,params(:,iteration));

%correct the raw data to the good radius
P_correct=P_correctU.*radius1;



%plotting
figure(1);
subplot(2,2,1);
hold on;
view(3);
plot3(P_samples(1,:),P_samples(2,:),P_samples(3,:),'.');
plot3(P_correct(1,:),P_correct(2,:),P_correct(3,:),'.');
legend("raw","calibrated");
title("Raw vs calibrated samples");
axis equal;

subplot(2,2,2);
hold on;
view(3);
plot3(x,y,z,".",MarkerSize=4);
plot3(pre_calibration(1,:),pre_calibration(2,:),pre_calibration(3,:),'.',MarkerSize=3);
plot3(P_correctU(1,:),P_correctU(2,:),P_correctU(3,:),'.');
legend("perfectly calibrated","pre-calibration","calibration at i= "+string(iteration)+" and a= "+string(alpha));
title("calibration to the unit sphere");
axis equal;

subplot(2,2,3);
hold on;
plot(I.^0.5);
plot(ones([1 iteration]).*noise*radius);
title("convergeance of the cost function")
legend("average distance","noise");


subplot(2,2,4);
hold on;
plot(get_rotation_costs(params));
plot(ones([1 iteration]).*min_rot_cost);
title("angular bias evolution");
legend("a(n)","limite");

%findAlpha(1e-3,1e3,1.3,iteration,n_learner);

%##############################
%##declarations des fonction###
%##############################



function ret=cost(parameters)%fonction de cout (relatif à distance avec la surface de la sphere unitaire)
global P_samples;
global radius1;
P_corrected=correct(P_samples,parameters);
avr=0;
n=length(P_corrected);
    for i=1:n
        avr=avr+ep(abs(norm(P_corrected(:,i))-1));
    end   
ret=avr/n*(radius1^2);
end

function ret=rotation_cost(parameters)%fonction de cout (relatif aux angles de rotation de la matrice de non-orthogonalité)
[A,B]=get_AB(parameters);
[~,~,nonOrtho,~] = explicit(A,B);%extract the non-orthogonality matrix
ret=(ep(dot(nonOrtho(1,:),[1 0 0])-1)+ep(dot(nonOrtho(2,:),[0 1 0])-1)+ep(dot(nonOrtho(3,:),[0 0 1])-1))/3;   
end

function ret=get_rotation_costs(parameters)
n=size(parameters,2);
ret=zeros(n,1);
for i=1:size(parameters,2)
    ret(i)=rotation_cost(parameters(:,i));
end
end



function ret = ep(x)%moindres carrés ? moindres ? on choisi ici
%ret=exp(-2*x)/2+x-1/2;
ret=x^2;
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

function P_corrected=correct(samples,parameters)
[AA,BB]=get_AB(parameters);
P_corrected=AA\(samples-BB);
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

%##############################
%######Fin du programme########
%##############################