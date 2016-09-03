function [ index, dist ] = get_gt( base, q )
% get ground truth( the euler distance between q and each columns of base)
% return a column vector which contains the sorted ground truth
% and another column vector which contains the distance related to the
% former one

[row, col] = size(base);
index = zeros(col, 1);
dist = zeros(col, 1);

for i = 1 : col
    v = base(:,i);
    index(i) = i;
    dist(i) = euler_dist(v,q);
end

rst = [index dist];
rst = sortrows(rst, 2);

index = rst(:,1);
dist = rst(:,2);

end

