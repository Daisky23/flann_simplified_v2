plot(tableNums, buildTimes/1000, '*k-');
axis([16,96,0,50]);
set(gca,'XTick',[16:16:96]);
xlabel('Code Length');
ylabel('Average Index Build Time(s)');
box on;
