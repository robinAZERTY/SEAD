clearvars;
clc;
diary ('myVariable.txt');
syms x y z 
syms a b x0 y0 z0 theta
syms xO yO zO xA yA zA xB yB zB xC yC zC xD yD zD xE yE zE 

%placer les points sur un pentagone presque régulier
points=[0,0,zO;0,-1,zA;1,round(1/tan(8*pi/5),3),zB;round(tan(6*pi/5),3),1,zC;round(tan(4*pi/5),3),1,zD;-1,round(-1/tan(2*pi/5),3),zE];
for i=1:size(points,1)
    points(i,1)=vpa(points(i,1));
    points(i,2)=vpa(points(i,2));
end



%ou juste des points quelconques
points=[xO yO zO;xA yA zA;xB yB zB;xC yC zC;xD yD zD;xE yE zE];

points=points
%equation du paraboloïde
eq = a*((x-x0)*cos(theta)+(y-y0)*sin(theta))^2+b*((x-x0)*sin(theta)-(y-y0)*cos(theta))^2+z0==z


%calculer la valeur en Z de ces points en choisissant des valeurs de x0 y0
%z0 a b et theta

%{
x0_y0_z0_a_b_theta=[-0.3,0.2,3,-0.03,-0.04,0.5];
new_expression=subs(eq,[x0 y0 z0 a b theta],x0_y0_z0_a_b_theta)

for i=1:size(points,1)
    points(i,1)=vpa(points(i,1));
    points(i,2)=vpa(points(i,2));
    points(i,3)=vpa(round(lhs(subs(new_expression,[x y],[points(i,1) points(i,2)])),4));
end
points
%plot the surface
[X,Y] = meshgrid(-1.5:0.1:1.5,-1.5:0.1:1.5);
Z= 3 - (2*(cos(1/2)*(Y - 1/5) - sin(1/2)*(X + 3/10)).^2)./5 - 2*(cos(1/2)*(X + 3/10) + sin(1/2)*(Y - 1/5)).^2;
figure();
surf(X,Y,Z);
axis square
%}
system=[a*0]*size(points,1);
for i=1:size(points,1)
    system(i)=subs(eq,[x y z],points(i,:));
end

unknown=[a b x0 y0 z0 theta]

system=transpose(system)

[a,b,x0,y0,z0,theta]=solve(system,unknown,MaxDegree=4);
a=vpa(simplify(a(1)))
b=vpa(simplify(b(1)))
x0=vpa(simplify(x0(1)))
y0=vpa(simplify(y0(1)))
z0=vpa(simplify(z0(1)))
theta=vpa(simplify(theta(1))) 


