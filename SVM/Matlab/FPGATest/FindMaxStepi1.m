function i1 = FindMaxStepi1(i2, C, alpha, E)

Emax = 0;
index = find(((alpha ~= C) & (alpha ~= 0)));
[m, ~] = size(index);
for i = 1 : m
    ETemp = abs(E(index(i)) - E(i2));
    if(ETemp > Emax)
        Emax = ETemp;
        i1 = index(i);
    end
end

i1 = 1;