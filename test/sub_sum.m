function yy = sub_sum(y,k)
yy = [];
j = 0;
tmp = 0;
for i=1:max(size(y))
    j = j+1;
    tmp = tmp + y(i);
    if(j==k)
        yy = [yy tmp];
        j = 0;tmp=0;
    end    
end
if(tmp>0)
    yy = [yy tmp];
end