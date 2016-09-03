function [ gt, gt_dist ] = get_groundtruth( base, query )

[row, base_col] = size(base);
[row, q_col] = size(query);

gt = [];
gt_dist = [];
for i = 1 : q_col
   q = query(:,i);
   [id,d] = get_gt(base, q);
   gt = [gt, id];
   gt_dist = [gt_dist, d];
end

end

