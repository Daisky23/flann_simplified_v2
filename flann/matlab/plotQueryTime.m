plot(tableNums, queryAverageTimes, '*k-');
axis([16,96,0,180]);
set(gca,'XTick',[16:16:96]);
xlabel('Code Length');
ylabel('Average Query Time(ms)');
box on;
