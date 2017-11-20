function [w, b] = FullSMO(xt, yt, C, tol)

numChanged = 0;
examineAll = 1;
[mxt, ~] = size(xt);

alpha = zeros(mxt, 1);
E = zeros(mxt, 1);
b = 0;

while(numChanged > 0 || examineAll)
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