plot(tableNums, maps, '*k-');
axis([16,96,0.05,0.4]);
set(gca,'XTick',[16:16:96]);
xlabel('Code Length');
ylabel('Mean Average Precision');
box on;
