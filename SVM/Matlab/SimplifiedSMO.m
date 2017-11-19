function [a, b] = SimplifiedSMO(C, tol, max_passes, xt, yt)
[m, n] = size(xt);

a = zeros([m, 1]); %Initialize parameters
b = 0;
passes = 0;
E = zeros([m, 1]);
a_old = zeros([m, 1]);


while(passes < max_passes)
    num_changed_alphas = 0;
    for i = 1 : m
        E(i) = fx(xt(i, :)', xt, yt, a, b) - yt(i);
        if((yt(i) * E(i) < -tol && a(i) < C) || (yt(i) * E(i) > tol && a(i) > 0))
            j = randi(m);
            while(j == i)
                j = randi(m);
            end
           E(j) = fx(xt(j, :)', xt, yt, a, b) - yt(j);
           a_old(i) = a(i);
           a_old(j) = a(j);
           %computer H & L
           if(yt(i) == yt(j))
               L = max(0, a(i) + a(j) - C);
               H = min(C, a(i) + a(j));
           else
               L = max(0, a(j) - a(i));
               H = min(C, C + a(j) - a(i));
           end
           if(L == H)
               continue;
           end
           %compute eta
           eta = 2 * (xt(i, :) * xt(j, :)') - (xt(i, :) * xt(i, :)') - (xt(j, :) * xt(j, :)');
           if(eta >= 0)
               continue;
           end
           %update a(j)
           a(j) = a(j) - ((yt(j) * (E(i) - E(j))) / eta);
           if(a(j) > H)
               a(j) = H;
           end
           if(a(j) < L)
               a(j) = L;
           end
           if(abs(a(j) - a_old(j)) < 10e-5)
               continue;
           end
           %determine ai
           a(i) = a(i) + yt(i) * yt(j) * (a_old(j) - a(j));
           %compute b1, b2, b
           b1 = b - E(i) - yt(i) * (a(i) - a_old(i)) * (xt(i, :) * xt(i, :)') - yt(j) * (a(j) - a_old(j)) * (xt(i, :) * xt(j, :)');
           b2 = b - E(j) - yt(i) * (a(i) - a_old(i)) * (xt(i, :) * xt(j, :)') - yt(j) * (a(j) - a_old(j)) * (xt(j, :) * xt(j, :)');
           if(a(i) > 0 && a(i) < C)
               b = b1;
           elseif(a(j) > 0 && a(j) < C)
               b = b2;
           else
               b = (b1 + b2) / 2;
           end
           num_changed_alphas = num_changed_alphas + 1;
        end
    end
    if(num_changed_alphas == 0)
        passes =  passes + 1;
    end
end

close all;
hold on;
for i = 1:m
    if(yt(i) == 1)
        plot(xt(i, 1), xt(i, 2), 'rx');
    end
    if(yt(i) == -1)
        plot(xt(i, 1), xt(i, 2), 'bo');
    end
end

w = (a.*yt)' * xt;

k = -w(1)/w(2);

xxx = [0 : 50];
plot(xxx, k*xxx + b);




