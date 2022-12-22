function [ret,iteration_number] = gradDescent(funcIn,varInit,eps,learningRate,stopRate)
ret=varInit;
iteration_number=0;
while true
    grad=numGrad(funcIn,ret,eps);
    slope=norm(grad);
    if slope<=stopRate
        break
    end
    ret=ret-grad.*learningRate;
    iteration_number=iteration_number+1;
end
end