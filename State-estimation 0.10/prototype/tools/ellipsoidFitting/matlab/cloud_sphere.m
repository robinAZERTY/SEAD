function [x,y,z]=cloud_sphere(n,noise)%generation de points bruités distribuées homogènement sur une sphere unitaire
rvals = 2*(rand(n,1)-0.5);
elevation = asin(rvals);
azimuth = 2*pi*rand(n,1);
radius= randn(n,1).*noise+1;
[x,y,z] = sph2cart(azimuth,elevation,radius);
end