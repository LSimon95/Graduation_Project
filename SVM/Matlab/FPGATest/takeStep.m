function [Result, alpha, b, E] = takeStep(i1, i2, xt, yt, C, alpha, b, E)
       
if(i1 == i2)
    Result = 0;
    return;
end 
a1 = alpha(i1);
a2 = alpha(i2); 
y1 = yt(i1);
y2 = yt(i2);


s = y1 * y2;
if(y1 == y2)
    L = max(0, a1 + a2 - C);
    H = min(C, a1 + a2);
else
    L = max(0, a2 - a1);
    H = min(C, C + a2 - a1);
end
if(L == H)
    Result = 0;
    return;
end

eta =  (xt(i1, :) * xt(i1, :)') + (xt(i2, :) * xt(i2, :)') - 2 * (xt(i1, :) * xt(i2, :)');
if(eta > 0)
    a2 = a2 + ((y2 * (E(i1) - E(i2))) / eta);
    if(a2 < L)
        a2 = L;
    elseif(a2 > H)
        a2 = H;
    end
else
    Result = 0;
    return;
end

if(abs(a2 - alpha(i2)) < 10e-5)
    Result = 0;
    return;
end

a1 = alpha(i1) + s*(alpha(i2) - a2);

 %compute b1, b2, b
b = 0;
b1 = E(i1) + y1 * (a1 - alpha(i1)) * (xt(i1, :) * xt(i1, :)') + y2 * (a2 - alpha(i2)) * (xt(i1, :) * xt(i2, :)') + b;
b2 = E(i2) + y1 * (a1 - alpha(i1)) * (xt(i1, :) * xt(i2, :)') + y2 * (a2 - alpha(i2)) * (xt(i2, :) * xt(i2, :)') + b;
if(a1 > 0 && a1 < C)
    b = b1;
elseif(a2 > 0 && a2 < C)
    b = b2;
else
    b = (b1 + b2) / 2;
end
%update w

%updata E
E(i1) = fx(xt(i1)) - y1;
E(i2) = fx(xt(i2)) - y2;

alpha(i1) = a1;
alpha(i2) = a2;

Result = 1;
return;


