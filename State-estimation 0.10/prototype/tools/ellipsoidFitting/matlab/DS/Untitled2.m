clearvars;
clc;
diary ('myVariable.txt');
syms x y z tau
syms a b c x0 y0 z0 tau0 theta
syms xO yO zO tauO xA yA zA tauA xB yB zB tauB xC yC zC tauC xD yD zD tauD xE yE zE tauE xF yF zF tauF xG yG zG tauG



%equation du paraboloïde
eq= a*((x-x0)*cos(theta)+(y-y0)*sin(theta))^2+b*((x-x0)*sin(theta)-(y-y0)*cos(theta))^2+ c*(tau-tau0)^2+z0==z

%développer et factoriser par x²,y²,xy,x,et y
z=expand(z);
%facteur de x²

A= children(collect(lhs(eq),[x y tau]));

xx_=combine(expand(A{1}/(x*x)),'sincos');
xy_=combine(expand(A{2}/(x*y)),'sincos');
x_=combine(expand(A{3}/x),'sincos');
yy_=combine(expand(A{4}/(y*y)),'sincos');
y_=combine(expand(A{5}/y),'sincos');
tautau_=expand(A{6}/(tau*tau));
tau_=expand(A{7}/tau);
o_=combine(expand(A{8}),'sincos');


factorisation_ok = ~logical(simplify(lhs(eq)-x*x*xx_-y*y*yy_-x*x_-y*y_-x*y*xy_-tautau_*tau*tau-tau_*tau-o_));

%points=[xO yO zO tauO; xA yA zA tauA ;xB yB zB tauB ;xC yC zC tauC ;xD yD zD tauD; xE yE zE tauE ;xF yF zF tauF ;xG yG zG tauG]
points=[0 0 3 0;0 -1 3 0;1 0 3 0;1 1 3 0;-1 1 3 0;-1 0 3 0;0 0 2 0;0 0 4 0];
%calculer leur valeur en z en choisissant des valeurs de a b c x0 y0 tau0 z0 et theta
a_b_c_x0_y0_z0_tau0_theta=[-1 -0.01 -1 0.1 0 5 3 -0.1];

true_eq=subs(eq,[a b c x0 y0 z0 tau0 theta],a_b_c_x0_y0_z0_tau0_theta)
for i=1:size(points,1)
    c_z=subs(lhs(true_eq),[x y tau],[points(i,1) points(i,2) points(i,3)]);
    points(i,4)=c_z;
end
points

A=[a*0 a*0 a*0 a*0 a*0 a*0 a*0 a*0]*size(points,1);
Y=[a*0;a*0;a*0;a*0;a*0;a*0;a*0;a*0];
for i=1:size(points,1)
    xx=points(i,1);
    yy=points(i,2);
    tautau=points(i,3);
    zz=points(i,4);
    A(i,:)=round(vpa([xx*xx yy*yy tautau*tautau xx*yy xx yy tautau 1]),5);
    Y(i)=round(vpa(zz),5);
end
det(A)
A
Y



syms X0 X1 X2 X3 X4 X5 X6 X7

X=[X0;X1;X2;X3;X4;X5;X6;X7]
XX=[xx_;yy_;tautau_;xy_;x_;y_;tau_;o_];
eq2= X==XX


[a_ b_ c_ x0_ y0_ z00_ tau0_ theta_]=solve(eq2,[a b c x0 y0 z0 tau0 theta]);
X=round(A^-1*Y,5)
txt='X=inv(A)*Y'

syms tmp1 tmp2

a_=a_(1);
b_=b_(1);
c_=c_(1);
x0_=x0_(1);
y0_=y0_(1);
tau0_=tau0_(1);
theta_=theta_(1);

litteralSolutions=[a_ b_ c_ x0_ y0_ tau0_ theta_]


tmp1=(X0^2 - 2*X0*X1 + X1^2 + X3^2)^(1/2);
tmp2=(- X3^2 + 4*X0*X1);


aa=subs(a_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
bb=subs(b_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
cc=subs(c_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
xx0=subs(x0_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
yy0=subs(y0_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
ttau0=subs(tau0_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
if abs(X(4))>1e-6
    ttheta=subs(theta_,[X0 X1 X2 X3 X4 X5 X6 X7],[X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8)]);
else 
    ttheta=0;
end

replace=[x y tau z a b c x0 y0 tau0 theta];
by=[points(1,1) points(1,2) points(1,3) points(1,4) aa bb cc xx0 yy0 ttau0 ttheta];
newExpr=subs(eq,replace,by);
zz0=solve(newExpr,z0);

numericalSolutions=round([aa bb cc xx0 yy0 zz0 ttau0 ttheta],5)


