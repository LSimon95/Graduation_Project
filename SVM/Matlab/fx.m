function f = fx(x, xt, yt, w, b, sigma)

[m, ~] = size(xt);

f = 0;
for i = 1 : m
    f = f + w(i) * yt(i) * Kernel(xt(i, :), x, sigma);
end

f = f + b;