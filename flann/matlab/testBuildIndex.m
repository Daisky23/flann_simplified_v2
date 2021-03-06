function [ tableNums, buildTimes, saveIndexTimes ] = testBuildIndex( base, from, to, interval )

tableNums = [];
buildTimes = [];
saveIndexTimes = [];
for i = from : interval : to
    filename = ['index', num2str(i), '.dat'];
    [buildTime, saveIndexTime] = buildIndexAndSave(base, filename, i);
    tableNums = [tableNums; i];
    buildTimes = [buildTimes; buildTime];
    saveIndexTimes = [saveIndexTimes; saveIndexTime];
end;

end

