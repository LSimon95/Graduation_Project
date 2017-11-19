clc
clear
close all
%creat data

%Linear
% ooo = ones([50, 1]);
% moo = zeros([50, 1]) - ooo;
% yt = [ooo;moo];
% xt = zeros([100, 2]);
% 
% for i= 1:50 %y > x
%     xt(i, 1) = i;
%     xt(i, 2) = (50 - i) * rand + i + 1;
% end
% 
% for i= 1:50 %y < x
%     xt(i + 50, 1) = i;
%     xt(i + 50, 2) = (i - 1) * rand;
% end
% 

% 
%Circuit
xt = zeros([200, 2]);
yt = ones([200, 1]);
for i = 1 : 200
    xt(i, 1) = 100 * rand;
    xt(i, 2) = 100 * rand;
    if(((xt(i, 1) - 50) ^ 2 +  (xt(i, 2) - 50) ^ 2) < 625)
        yt(i) = -1;
    end
end

hold on;
[m, n] = size(xt);
for i = 1:m
    if(yt(i) == 1)
        plot(xt(i, 1), xt(i, 2), 'rx');
    end
    if(yt(i) == -1)
        plot(xt(i, 1), xt(i, 2), 'bo');
    end
end

save("data\d1x.mat", 'xt');
save("data\d1y.mat", 'yt');
saveas(1, 'data\im.png');
clc
clear