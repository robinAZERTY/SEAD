%a dual variables exemple
function ret = hypersquared(variables)
    if(size(variables,1)==2) 
        ret = variables(1,:).^2+variables(2,:).^2;
    else
        ret = nan;
    end

end