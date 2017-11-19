function [a, b] = SimplifiedSMOKer(C, tol, max_passes, xt, yt)
[m, ~] = size(xt);

a = zeros([m, 1]); %Initialize parameters
b = 0;
passes = 0;
E = zeros([m, 1]);
a_old = zeros([m, 1]);
sigma = 0.06;

while(passes < max_passes)
    num_changed_alphas = 0;
    for i = 1 : m
        E(i) = fx(xt(i, :), xt, yt, a, b, sigma) - yt(i);
        if((yt(i) * E(i) < -tol && a(i) < C) || (yt(i) * E(i) > tol && a(i) > 0))
            j = randi(m);
            while(j == i)
                j = randi(m);
            end
           E(j) = fx(xt(j, :), xt, yt, a, b, sigma) - yt(j);
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
           eta = 2 * Kernel(xt(i, :), xt(j, :), sigma) - Kernel(xt(i, :), xt(i, :), sigma) - Kernel(xt(j, :), xt(j, :), sigma);
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
           b1 = b - E(i) - yt(i) * (a(i) - a_old(i)) * Kernel(xt(i, :), xt(i, :), sigma) - yt(j) * (a(j) - a_old(j)) * Kernel(xt(i, :), xt(j, :), sigma);
           b2 = b - E(j) - yt(i) * (a(i) - a_old(i)) * Kernel(xt(i, :), xt(j, :), sigma) - yt(j) * (a(j) - a_old(j)) * Kernel(xt(j, :), xt(j, :), sigma);
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
    if(a(i) > 0 && a(i) < C)
        plot(xt(i, 1), xt(i, 2), 'gO');
    end
    if(yt(i) == 1)
        plot(xt(i, 1), xt(i, 2), 'rx');
    end
    if(yt(i) == -1)
        plot(xt(i, 1), xt(i, 2), 'b.');
    end
end
