function dist = euler_dist(a,b)
% need 2 column vectors, whose dimensions are identical
% return the euler distance between them

[row, col] = size(a);
dist = 0;
for i = 1 : row
    dist = dist + (a(i) - b(i))*(a(i) - b(i));
end
dist = sqrt(dist);

end

