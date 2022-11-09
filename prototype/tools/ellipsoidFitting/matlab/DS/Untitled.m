clearvars;
clc;
diary ('myVariable1.txt');
syms x y z
syms a b x0 y0 z0 theta
syms xO yO zO xA yA zA xB yB zB xC yC zC xD yD zD xE yE zE xF yF zF xG yG zG



%equation du paraboloïde
eq= a*((x-x0)*cos(theta)+(y-y0)*sin(theta))^2+b*((x-x0)*sin(theta)-(y-y0)*cos(theta))^2+z0==z

%développer et factoriser par x²,y²,xy,x,et y
z=expand(z);
%facteur de x²

A= children(collect(lhs(eq),[x y]))

xx_=combine(expand(A{1}/(x*x)),'sincos');
xy_=combine(expand(A{2}/(x*y)),'sincos');
x_=combine(expand(A{3}/x),'sincos');
yy_=combine(expand(A{4}/(y*y)),'sincos');
y_=combine(expand(A{5}/y),'sincos');
o_=combine(expand(A{6}),'sincos');


factorisation_ok = ~logical(simplify(lhs(eq)-x*x*xx_-y*y*yy_-x*x_-y*y_-x*y*xy_-o_))

%points=[xO yO zO tauO; xA yA zA tauA ;xB yB zB tauB ;xC yC zC tauC ;xD yD zD tauD; xE yE zE tauE ;xF yF zF tauF ;xG yG zG tauG]
points=[0 0 3;0 -1 3;1 0 3;1 1 3;-1 1 3;-1 0 3];
%calculer leur valeur en z en choisissant des valeurs de a b c x0 y0 tau0 z0 et theta
a_b_x0_y0_z0_theta=[-1 2 0.1 0 5 0.1];

true_eq=subs(eq,[a b x0 y0 z0 theta],a_b_x0_y0_z0_theta)
for i=1:size(points,1)
    c_z=subs(lhs(true_eq),[x y],[points(i,1) points(i,2)]);
    points(i,3)=c_z;
end
points

A=[a*0 a*0 a*0 a*0 a*0 a*0]*size(points,1);
Y=[a*0;a*0;a*0;a*0;a*0;a*0];
for i=1:size(points,1)
    xx=points(i,1);
    yy=points(i,2);
    zz=points(i,3);
    A(i,:)=round(vpa([xx*xx yy*yy xx*yy xx yy 1]),5);
    Y(i)=round(vpa(zz),5);
end
det(A)
A
Y



syms X0 X1 X2 X3 X4 X5

X=[X0;X1;X2;X3;X4;X5]
XX=[xx_;yy_;xy_;x_;y_;o_];
eq2= X==XX


[a_ b_ x0_ y0_ z00_ theta_]=solve(eq2,[a b x0 y0 z0 theta]);
X=round(A^-1*Y,5)
txt='X=inv(A)*Y'

syms tmp1 tmp2


a_=subs(a_(1),[(X0^2 - 2*X0*X1 + X1^2 + X2^2)^(1/2) (- X2^2 + 4*X0*X1)],[tmp1 tmp2])
b_=subs(b_(1),[(X0^2 - 2*X0*X1 + X1^2 + X2^2)^(1/2) (- X2^2 + 4*X0*X1)],[tmp1 tmp2])
x0_=subs(x0_(1),[(X0^2 - 2*X0*X1 + X1^2 + X2^2)^(1/2) (- X2^2 + 4*X0*X1)],[tmp1 tmp2])
y0_=subs(y0_(1),[(X0^2 - 2*X0*X1 + X1^2 + X2^2)^(1/2) (- X2^2 + 4*X0*X1)],[tmp1 tmp2])
theta_=subs(theta_(1),[(X0^2 - 2*X0*X1 + X1^2 + X2^2)^(1/2) (- X2^2 + 4*X0*X1)],[tmp1 tmp2])

litteralSolutions=[a_ b_ x0_ y0_ theta_];

tmp1=(X0^2 - 2*X0*X1 + X1^2 + X3^2)^(1/2)
tmp2=(- X2^2 + 4*X0*X1)

ttmp1= subs(tmp1,[X0 X1 X2 X3 X4 X5],[X(1) X(2) X(3) X(4) X(5) X(6)])
ttmp2= subs(tmp2,[X0 X1 X2 X3 X4 X5],[X(1) X(2) X(3) X(4) X(5) X(6)]);

aa=subs(a_,[X0 X1 X2 X3 X4 X5 tmp1 tmp2],[X(1) X(2) X(3) X(4) X(5) X(6) ttmp1 ttmp2])
bb=subs(b_,[X0 X1 X2 X3 X4 X5 tmp1 tmp2],[X(1) X(2) X(3) X(4) X(5) X(6) ttmp1 ttmp2]);
xx0=subs(x0_,[X0 X1 X2 X3 X4 X5 tmp1 tmp2],[X(1) X(2) X(3) X(4) X(5) X(6) ttmp1 ttmp2]);
yy0=subs(y0_,[X0 X1 X2 X3 X4 X5 tmp1 tmp2],[X(1) X(2) X(3) X(4) X(5) X(6) ttmp1 ttmp2]);
if abs(X(3))>1e-6
    ttheta=subs(theta_,[X0 X1 X2 X3 X4 X5 tmp1 tmp2],[X(1) X(2) X(3) X(4) X(5) X(6) ttmp1 ttmp2]);
else 
    ttheta=0;
end

replace=[x y z a b x0 y0 theta];
by=[points(1,1) points(1,2) points(1,3) aa bb xx0 yy0 ttheta];
newExpr=subs(eq,replace,by);
zz0=solve(newExpr,z0);

numericalSolutions=round([aa bb xx0 yy0 zz0 ttheta],5)


