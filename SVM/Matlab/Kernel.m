function result = Kernel(x, z, sigma)

result = exp(-norm(x - z) / 2 * sigma);

end