数据预处理

<br>

##	Outline

*	概述，Abstract，什么是预处理，为什么，怎么办

*	数据清洗，data cleaning

*	数据整合，data integration

*	压缩，reduction

*	变换，离散化

	<br>

##	概述

*	为什么

	如果不进行预处理，数据的质量一般会很糟糕，会带来维度灾难等后果，影响后续处理的效率以及最终结果。

	<br>

*	四个步骤

	清洗：填充缺失值，去噪降噪，去除异常点

	整合：整合不同来源的数据

	压缩：压缩

	变换：分层，归一化

	<br>

##	数据清洗

*	原始数据可能存在哪些问题？

	噪声，离群点，缺失值，重复值

	<br>

*	如何去噪

	均值，最大值，最小值，中值

	回归，用超平面拟合数据点，

	<br>

*	离群点

	明显脱离集体的点

	<br>

*	缺失值

	信息收集不完全，丢失，。。。

	如何处理：舍弃，估计（0，众数，中值，。。。）

	<br>

*	重复值

	融合两个数据集的时候容易出现重复，比如某个人有多个邮箱地址，多次填表，。。。

	去重

	<br>

##	数据整合

*	整合不同来源的数据，同时要检查冲突

	相同的实体，在不同的数据集中可能有不同的表达方式，比如名字不同，长度单位不同，阿拉伯数字或者英文数字表达

	<br>

*	处理数据冗余

	对于不同表达方式的对象，要能识别出最终实体

	月收入 * 12 ≈ 年收入

	处理方式：相关性分析 correlation analysis, 协方差分析 covariance analysis

	<br>

*	如果处理得当，可以减少冗余，减少维度，提升数据集质量，提升处理速度

	<br>

*	相关性分析 correlation analysis

	*	X^2 (Chi-Square) test，卡方检验（存疑）

		![correlation analysis](https://segmentfault.com/img/bVbrNlz)

		X^2 越大，相关性越大，> 10.828 说明明显相关

		![X2](https://segmentfault.com/img/bVbrNlR)

		<br>

	*	相关系数

		![coefficient](https://segmentfault.com/img/bVbrNmc)

		r > 0, 正相关

		r = 0, 不相关

		r < 0, 负相关

		<br>

	*	相关性

		先对 a, b 标准化 a = (a - mean_a) / std_a, b = (b - mean_b) / std_b

		correlation(A, B) = a * b (内积)

		<br>

*	协方差分析

	![Covariance](https://segmentfault.com/img/bVbrNmK)

	cov(A, B) > 0, 正相关, < 0 负相关, 

	例子

	![demo cov(A, B)](https://segmentfault.com/img/bVbrNnq)

	<br>

##	压缩

*	使数据体积减小，但是不影响最终分析结果

	<br>

*	为什么要压缩：数据集的体积太大了，处理时间太长

	<br>

*	维度灾难

	当维度增加，数据会变得更加系数，体积明显增加，组合数爆炸式增长，使维度增加前的一些定义失去意义，可视化难度增加

	<br>

*	策略

	降维（移除无用属性）：小波变换，PCA，特征选择

	替换：用超平面拟合数据，只保留超平面参数

	压缩：

	变换：傅里叶变换，小波变换（图像压缩，多分辨率分析）

	小波分解

	![wavelet1](https://segmentfault.com/img/bVbrNpf)

	![wavelet2](https://segmentfault.com/img/bVbrNpg)

	<br>

*	为什么用小波（存疑）

	*	滤波器(hat-shape filter)：突出那些点集中的区域，抑制边界点

	*	移除离群点，过滤噪声

	*	多分辨率，检测不同尺度下不同形状的集群

	*	高效，O(n)

	*	但是只对低维数据有效

		<br>

*	PCA, Principal Component Analysis, 主成分分析

	把高维空间映射到低维空间，同时尽可能多地保留原来的点

	找出协方差矩阵的特征向量，这些特征向量定义了新的空间

	步骤：先进行标准化，使所有属性的值域相同；计算 k 个标准正交向量，即 k 个主要成分；排序，舍弃较小的向量，从而达到降维的目的

	只对数值型数据有效

	<br>

*	特征选择

	舍弃一些特征，比如 总价 = 单价 * 数量，总价其实可以舍去。还有就是一些明显不相关的特征，比如学号和成绩一般来说是不相关的，学号可以舍去。

##	其他数据压缩方法

*	假设数据分布符合某种模型，我们可以用超平面拟合数据，然后只保留参数。

	线性回归（假设初始直线，计算所有点到直线的距离的平方和作为误差 sqe，梯度下降使得sqe最小）： Y = wX + b

	多元回归： Y = b0 + b1X1 + b2X2 + ...

	不止是数据压缩降维，还可以使数据平滑

	<br>

*	直方图分析

	把数据放到不同的 bucket/bin ，这些 bucket 都是计数器，用统计值（求和，平均值）代替 bucket 中的数据

	等宽(equal-width), 0-10, 10-20, 20-30, ...

	等间隔(equal-frequency)，每 5 个人一个 bucket，

	<br>

*	聚类

	先聚类，然后只保留类中心或者类特征，以此代表整个类的点

	<br>

*	抽样（样本代替总体）

	随机采样，放回采样，不放回采样，自适应采样(如分层采样，避免倾斜)

	注意，不会减少IO时间

	<br>

*	数据压缩

	有损/无损 压缩

	文本压缩（通常无损），音视频压缩（通常有损）

	<br>

##	数据变换

*	平滑（去噪）

	<br>

*	把两个特征合并为新的特征

	<br>

*	聚合：摘要，数据方快

	<br>

*	标准化

	min-max，z-score(减去均值, 再除以标准差)，decimal scaling

	![normalization](https://segmentfault.com/img/bVbrNvr)

	<br>

*	离散化

	<br>

##	离散化

*	把【1.非数值型数据，2.连续的数值型数据】转为【离散的数值型数据】

	【1.非数值型数据】：Nominal（名词，如颜色，职业），Ordinal（序数词）

	【2.连续的数值型数据】：量化，离散化，以此减少数据量。为后续的分类做准备。

	<br>

*	方法

	*	Binning, 分桶，直方图，top-down，无监督。

		*	Equal-width (distance) partitioning，相同大小的全部放在一个桶。

			噪点影响较大，可能存在严重倾斜

		*	Equal-depth (frequency) partitioning，每个桶的数量相等

		*	Smoothing

			放到不同桶里面，然后用均值/最大值/最小值进行平滑

			<br>

	*	聚类，无监督，top-down split 或 bottom-up merge

	*	决策树，有监督，top-down

	*	相关性分析，无监督，bottom-up

		<br>

##	分层

*	可能需要人为制定划分标准，street < city < state < country

*	也可以把取值最多的属性作为最低层, 比如街道名的取值一般要远远大于国家名的取值

	![Automatic Concept Hierarchy Generation](https://segmentfault.com/img/bVbrNx8)

	<br>

##	小结

*	数据清洗：去掉重复值，异常值，缺失值，噪声

*	数据整合：整合不同来源的数据，处理冲突，相关性分析，协方差分析

*	数据压缩：去掉无用信息，改变表达方式，降维，减少数据体积，加快处理速度，还能产生相同的分析结果

*	数据变换：标准化，离散化，改变属性表达方式，映射到新的属性空间

	<br>
