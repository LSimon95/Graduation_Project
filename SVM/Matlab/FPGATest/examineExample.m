function [Result, alpha, b, E] = examineExample(i2, xt, yt, alpha, C, tol, b, E)

y2 = yt(i2);
a2 = alpha(i2);

E(i2) = fx(xt(i2, :), xt, yt, alpha, b) - y2;
r2 = E(i2) * y2;

if((r2 < -tol && a2 < C) || (r2 > tol && a2 > 0))
    if(~isempty(find(((alpha ~= C) & (alpha ~= 0)), 1)))
        i1 = FindMaxStepi1(i2, C, alpha, E);
        [ResultTakeStep, alpha, b, E] = takeStep(i1, i2, xt, yt, C, alpha, b, E);
        if(ResultTakeStep)
            Result = 1;
            return;
        end
    end
    %loop over all non-zero and non-c alpha, starting at a random point
    index = find(((alpha ~= C) & (alpha ~= 0)));
    [m, ~] = size(index);
    for i = 1 : m
        index = index(randperm(m));
        i1 = index(i);
        [ResultTakeStep, alpha, b, E] = takeStep(i1, i2, xt, yt, C, alpha, b, E);
        if(ResultTakeStep)
            Result = 1;
            return;
        end
    end
    %loop over all possible all i1, staring at a random point
    [m, ~] = size(xt);
    index = randperm(m);
    for i = 1 : m
        i1 = index(i);
        [ResultTakeStep, alpha, b, E] = takeStep(i1, i2, xt, yt, C, alpha, b, E);
        if(ResultTakeStep)
            Result = 1;
            return;
        end
    end
end
Result = 0;
         