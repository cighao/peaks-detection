clear;clc;close all;
y = importdata('data1.txt');  % 导入数据
[peak, ~] = find_peak(y,1);   % 波峰
[trough, yy] = find_peak(y,2);  % 波谷
%输出结果
disp('波峰点的序号')
peak
disp('波谷点的序号')
trough
%图形化显示
figure(1);
h = plot(1:size(yy,2),yy);
xlabel('序号');
ylabel('峰值');
% title('滤波后的图形')
% set(h,'Color',[1 0 1])
hold on;
for i=1:size(peak,2)  % 根据结果标记波峰位置
    tmpx = peak(i);
    tmpy = yy(tmpx);
    h = plot(tmpx,tmpy,'o');
    set(h,'MarkerFaceColor',[1 0 0])
%     text(tmpx+2,tmpy,['(' num2str(tmpx) ',' num2str(tmpy) ')'],'FontSize',3);
     text(tmpx+5,tmpy, num2str(tmpx),'FontSize',6);
end
for i=1:size(trough,2)  % 根据结果标记波谷的位置
    tmpx = trough(i);
    tmpy = yy(tmpx);
    h = plot(tmpx,tmpy,'o');
    set(h,'MarkerFaceColor',[0 1 0])
%     text(tmpx+2,tmpy,['(' num2str(tmpx) ',' num2str(tmpy) ')'],'FontSize',3);
    text(tmpx+5,tmpy, num2str(tmpx),'FontSize',6);
end