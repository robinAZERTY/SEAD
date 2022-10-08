%exlicit the A and B of a ellispoid
function [radius,gains,nonOrtho,offsets] = explicit(A,B)
offsets=B;

radius=(norm(A(1,:))+norm(A(2,:))+norm(A(2,:)))/3;
gains=[norm(A(1,:));norm(A(2,:));norm(A(2,:))]./radius;

nonOrtho=[A(1,:)./norm(A(1,:));A(2,:)./norm(A(2,:));A(3,:)./norm(A(3,:))];
end