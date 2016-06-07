function [peak yy] = find_peak(y,flag)
% 求y的波峰或者波谷
% y: 输入的序列
% yy: 滤波后的数据
% peak：峰值点所在的序号
% flag: 1-->表示求波峰;2-->表示求波谷

% step 0: 预处理
if (flag==2)
    y = y .* -1;
end

% step 1: 滤波
% yy = median_filter(y,5);
yy = mean_filter(y,6);
% yy = median_filter(yy,9);

% step 2: 求极大值点
[~, tmp_peak] = findpeaks(yy);

% step 3: 阈值策略峰值点序号
max_y = max(yy);
e = (max(yy)-min(yy))*0.4;    %设定阈值
R = 35;      %搜索半径
peak=[];
for i=1:size(tmp_peak,2)
    if(max_y - yy(tmp_peak(i)) <= e) %是否满足阈值
        found = 1;
    else 
        continue;
    end
    for j = tmp_peak(i)-R:tmp_peak(i)+R %在领域内是否是最大值，这里假设领域半径为40
       if(j<1 || j>size(yy,2))
           continue;
       end
       if(yy(tmp_peak(i))<yy(j))
           found = 0;
       end
       if(yy(tmp_peak(i))==yy(j) && sum(peak==j)==1)
           found = 0;
           break;
       end           
    end
    if(found==1)   
        peak = [peak tmp_peak(i)];%记录该波峰的序号
    end
end

if(flag==2)
    yy = -1.*yy;
end
