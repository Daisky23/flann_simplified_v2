# flann_simplified_v2
在上一阶段的工作中，我已初步完成了简化版flann库的构建工作，实现了一个基于夹角余弦的LSH，能够接受浮点类型数据输入，并实现了一个简单的Matlab接口进行调试。通过在SIFT1M、GIST1M数据集上的测试，证明算法运行良好，并就参数对算法效果的影响做了一定的分析。

然而在实际中往往更常用欧氏距离，因而从算法原理上看，之前实现的基于夹角余弦的LSH存在一定先天的遗憾，故此在这一阶段的工作中，主要重新研究实现了基于欧式距离的LSH算法，改用基于p-stable分布的E2LSH算法。

另外，针对上一阶段工作中构建好的索引无法保存的问题，实现了一组索引文件存取接口，能够将构建好的索引以二进制文件形式保存，并在需要时读取使用，通过在SIFT1M数据集上的测试，检验了索引文件存取的速度，效果令人较为满意。

最后，设计实验，在SIFT1M数据集上测试了LSH算法与flann库所实现的kd-tree算法的性能，并做了比较分析。

具体工作的细节详见report，已随邮件附件发送给您。
　　
# Quick Start
请在matlab中进行测试实验，flann/matlab目录下提供了三个matlab接口程序knn.cpp、buildIndexAndSave.cpp、loadIndexAndQuery.cpp，以及若干matlab测试函数，同时包含了 http://corpus-texmex.irisa.fr 所提供的fvecs、ivecs数据集读取函数。

测试前，请在matlab中先读取好测试用的dataset、query、groundtruth矩阵，由于实验方案中每次查询返回的点集为20,000个，而官方所提供的groundtruth数据矩阵只为每个查询点提供了排名前100的结果点集，因此为获取实验数据需要自行构建包含查询点排名前20,000的匹配数据点，这里我编写了一个matlab函数get_groundtruth，可以获取一个查询点集内每个点完整的groundtruth，函数使用方法如下：

<pre>[ gt, gt_dist ] = get_groundtruth( base, query )</pre>

返回的gt矩阵每一列包含对应查询点的完整gruondtruth索引，而gt_dist矩阵则包含了与gt矩阵每一个元素相对应的查询点-数据点欧氏距离，之后可自行截取所需的前20,000行。（由于数据集很大，该操作较为耗时，慎用）

在matlab中使用mex命令编译好三个接口程序后，可利用接口进行实验。三个接口用法如下：

knn.cpp接口输入原始数据点集base、查询点集query，返回前20,000个匹配结果点的索引矩阵index及其与查询点的欧式距离矩阵dist：

<pre>[ index, dist ] = knn( base, query )</pre>

buildIndexAndSave.cpp接口利用输入的原始数据矩阵构建索引，并将结果保存到指定文件，返回构建索引的耗时及保存索引文件耗时：

<pre>[ buildIndexTime, saveIndexTime ] = buildIndexAndSave( base, filename, indexHashTableNumber )</pre>

loadIndexAndQuery.cpp接口从指定文件中读取索引，结合输入的数据点集还原索引，并利用输入的查询点集进行查询，返回加载索引文件耗时及查询平均耗时：

<pre>[ index, dist, loadIndexTime, averageQueryTime ] = loadIndexAndQuery( filename, base, query )</pre>

另提供两个matlab下的测试函数testBuildIndex、testLoadIndex，使用方法如下：

testBuildIndex利用数据点集base构建若干不同哈希编码长度的索引并保存，from、to、interval控制哈希编码长度的范围和间隔，哈希编码长度为L的索引文件将保存到工作目录下的indexL.dat文件中，函数返回构建这一组索引的耗时和保存索引的耗时：

<pre>[ tableNums, buildTimes, saveIndexTimes ] = testBuildIndex( base, from, to, interval )</pre>

testLoadIndex利用from、to、interval控制依次读取testBuildIndex所保存的一系列索引文件，并利用查询点集query做查询，返回利用各组不同索引进行查询的平均时间、读取索引耗时、查询结果的平均准确率：

<pre>[tableNums, loadIndexTimes, queryAverageTimes, maps] = testLoadIndex(base, from, to, interval, query, gt)</pre>

此外还提供了plotIndexBuildTime、plotIndexLoadTime、plotIndexSaveTime、plotQueryTime、plotMAP等一系列脚本文件，辅助在使用上述两个测试函数后进行绘图。
