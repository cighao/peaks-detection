clear;clc;close all;
y = importdata('data1.txt');
y = y;
n = max(size(y));
% avg = sum(y)/n;
% y_std = std(y);
% y = (y - ones(1,n)*avg)./y_std;
% yy = median_filter(y,5);
yy = mean_filter(y,5);

max_y = max(yy);
e = (max(yy)-min(yy))*0.4;
plot(1:size(yy,2),yy)
[~, a] = findpeaks(yy);
% a
peak=[];
for i=1:size(a,2)
    if(max_y - yy(a(i)) <= e)
        found = 1;
    else 
        continue;
    end
    for j = a(i)-40:a(i)+40
       if(j<1 || j>size(yy,2))
           continue;
       end
       if(yy(a(i))<yy(j))
           found = 0;
       end
    end
    if(found==1)
        peak = [peak a(i)];
    end
end
peak


% plot(x,y)
% yy = mean_filter(y,11);
% % plot(1:size(yy,2),yy);
% yy = diff(yy);
% yy = sign(yy);
% yy = diff(yy);
% yy = sign(yy);
% % a = find(yy<=-1)+2
% [~,a] = findpeaks(y);
% max_y = max(y)*0.8;
% for i=1:size(a,2)
%     if(y(a(i))>max_y)
%         a(i)
%     end
% end