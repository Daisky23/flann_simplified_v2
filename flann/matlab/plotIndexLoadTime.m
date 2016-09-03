plot(tableNums, loadIndexTimes/1000, '*k-');
axis([16,96,0,8]);
set(gca,'XTick',[16:16:96]);
xlabel('Code Length');
ylabel('Loading Index Time Consuming(s)');
box on;
