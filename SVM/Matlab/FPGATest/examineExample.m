function [Result, alpha, b] = examineExample(i2, xt, yt, alpha, tol, C)

y2 = yt(i2);
a2 = alpha(i2);
E2 = fx(xt(i2)) - y2;
r2 = E2 * y2;

if((r2 < -tol && a2 < c) || (r2 > tol && a2 > 0))
    if(~isempty(find(((alpha ~= C) & (alpha ~= 0)), 1)))
    end
end
         