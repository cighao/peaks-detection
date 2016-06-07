function yy = mean_filter(y,k)
% 均值滤波
% yy: 滤波后的序列
% y：输入序列
% k: 求均值的子序列长度
n = max(size(y));
for i=1:k
    tmp(:,i) = y(i:n-k+i);
end
yy = [];
for i=1:size(tmp,1)
    yy = [yy sum(tmp(i,:))/k];
end