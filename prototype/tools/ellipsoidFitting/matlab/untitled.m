clc;

n=100; %nombre d'échantillons
global noise;
global P_real;
global P_samples;
global boundaries;
global iteration;
global P_correct;
global n_learner;
global alpha;

noise=0.00;
radius=5;
maxGainNoise=0.4;
maxNonOrtho=0.3;
maxOffset=30;


[x,y,z]=cloud_sphere(n,noise);
P_real=transpose([x y z]);


[A,B]=ellipse_rand(radius,maxGainNoise,maxNonOrtho,maxOffset)
[radius,gains,nonOrtho,offsets] = explicit(A,B);
P_samples=A*P_real+B;
transpose(P_samples)
[radius1,gains1,offsets1]=firstApprox(P_samples);


figure(1);
subplot(2,2,1);
plot3(P_samples(1,:),P_samples(2,:),P_samples(3,:),'.');
legend("Raw samples");
axis equal;
subplot(2,2,2);
plot3(x,y,z,".",color=[0.1,0.6,0.2],MarkerSize=4);
legend("perfectly calibrated");
axis equal;

function [x,y,z]=cloud_sphere(n,noise)
rvals = 2*(rand(n,1)-0.5);
elevation = asin(rvals);
azimuth = 2*pi*rand(n,1);
radius= randn(n,1).*noise+1;
[x,y,z] = sph2cart(azimuth,elevation,radius);
end

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
ret=ret;
end



function ret = ep(x)
%ret=exp(-2*x)/2+x-1/2;
ret=x^2;
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
    pause(1/t);
end

params=params(:,length(params)-1);
[A_est,B_est]=get_AB(params)
[radius,gains,nonOrtho,offsets]=explicit(A_est,B_est)
Error=I^0.5
end
