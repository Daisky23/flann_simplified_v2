function [tableNums, loadIndexTimes, queryAverageTimes, maps] = testLoadIndex(base, from, to, interval, query, gt)

tableNums = [];
loadIndexTimes = [];
queryAverageTimes = [];
maps = [];
for i = from : interval : to
    filename = ['index', num2str(i), '.dat'];
    [index, dist, loadIndexTime, queryAverageTime] = loadIndexAndQuery(filename,base,query);
    tableNums = [tableNums; i];
    loadIndexTimes = [loadIndexTimes; loadIndexTime];
    queryAverageTimes = [queryAverageTimes; queryAverageTime];
    
    [q_row, q_col] = size(query);
    sum_precision = 0;
    for j = 1 : q_col
        worst = gt(20000, j);
        precision = 0;
        for k = 1 : 20000
            if (worst >= dist(k, j) && worst < dist(k + 1, j))
                precision = k / 20000;
                break;
            end
        end
        sum_precision = sum_precision + precision;
    end
    map = sum_precision / q_col;
    maps = [maps; map];
    
end
