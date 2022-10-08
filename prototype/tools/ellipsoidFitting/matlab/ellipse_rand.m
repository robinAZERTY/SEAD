%generate a random ellipse parameters
function [A,B]=ellipse_rand(radius,maxGainNoise,maxNonOrtho,maxOffset)
B =(rand([3 1])-ones([3 1])*0.5).*2*maxOffset;
nonOrtho=(rand([3 3])-ones([3 3])*0.5).*maxNonOrtho+eye([3 3]);
nonOrtho=nonOrtho./((norm(nonOrtho(1,:))+norm(nonOrtho(2,:))+norm(nonOrtho(2,:)))/3);
trueRadius=radius+(rand([1 3])-0.5)*2*maxGainNoise;
A=nonOrtho.*trueRadius;
end