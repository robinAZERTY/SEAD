function [radius,gains,offsets]=firstApprox(rawPoints)
x=rawPoints(1,:);
y=rawPoints(2,:);
z=rawPoints(3,:);

offsets=[average(x);average(y);average(z)];

ampX=max(x)-min(x);
ampY=max(y)-min(y);
ampZ=max(z)-min(z);

radius = (ampX+ampY+ampZ)/3.0/2;

gains=[ampX;ampY;ampZ]./(radius*2);
end