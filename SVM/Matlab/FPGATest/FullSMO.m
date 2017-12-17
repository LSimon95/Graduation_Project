function [w, b] = FullSMO(xt, yt, C, tol)

numChanged = 0;
examineAll = 1;
[mxt, ~] = size(xt);

alpha = zeros(mxt, 1);
E = zeros(mxt, 1);
b = 0;
while(numChanged > 0 || examineAll == 1)
    numChanged = 0;
    if(examineAll)
        for i = 1 : mxt
            [ResultExamine, alpha, b, E] = examineExample(i, xt, yt, alpha, C, tol, b, E);
            numChanged = numChanged + ResultExamine;
        end
    else
        index = find(((alpha ~= C) & (alpha ~= 0)));
        [m, ~] = size(index);
        for i = 1 : m
            [ResultExamine, alpha, b, E] = examineExample(i, xt, yt, alpha, C, tol, b, E);
            numChanged = numChanged + ResultExamine;
        end
    end
    
    if(examineAll == 1)
        examineAll = 0;
    elseif(numChanged == 0)
        examineAll = 1;
    end
end

w = ((yt .* alpha)' * xt);

close all;
hold on;
for i = 1:mxt
    if(yt(i) == 1)
        plot(xt(i, 1), xt(i, 2), 'rx');
    end
    if(yt(i) == -1)
        plot(xt(i, 1), xt(i, 2), 'bo');
    end
end

k = -w(1)/w(2);

xxx = [0 : 50];
plot(xxx, k*xxx + b);
