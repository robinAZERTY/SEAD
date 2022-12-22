function ret=randVect(boundaries)
min_X=boundaries(:,1);
max_X=boundaries(:,2);
dim=size(min_X);
ret=zeros(dim);
    for j = 1:max(dim)
        ret(j)=rand()*(max_X(j)-min_X(j))+min_X(j);
    end
end
