function f = fx(x, xt, yt, alpha, b)

f = ((yt .* alpha)' * xt) * x' - b;