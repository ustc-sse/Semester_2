数据

<br>

##	四个问题

*	数据类型（用什么类型，用什么工具）

	<br>

*	数据质量（噪声，离散点，缺失值，异常点，重复）

	<br>

*	预处理（提高数据质量，使数据可以被特定的工具处理）

	<br>

*	相似性和距离度量

	<br><br><br>

##	本节大纲

*	关于数据的基本概念

	<br>

*	如何用统计学方法描述数据

	<br>

*	数据可视化

	<br>

*	度量数据之间的相似性和距离

	<br>

##	基本概念

*	数据集

	特性：维度，稀疏程度

	类型：结构化记录，图像，时间序列数据

	<br>

*	属性

	种类：分类数据（Categorical），数值型/量化数据（Quantitative），离散、连续

	<br>

##	统计学描述

*	Mean, 均值

	<br>

*	加权均值

	<br>

*	Median, 中位数

	<br>

*	Mode, 众数

	<br>

*	Midrange, 中列数 = `(min + max) / 2`

	<br>

####	对称性

*	Skewness, 倾斜程度 = Mean - Mode, Skewness

	Skewness = 0, 对称, Symmetric

	<br>

	![Symmetric](https://segmentfault.com/img/bVbrMwH?w=625&h=508)

	<br>

	Skewness > 0, 均值大于众数, 正向倾斜, Positively skewness

	![postive](https://segmentfault.com/img/bVbrMwU)

	<br>

	Skewness < 0, 均值小于众数, 负向倾斜, Negatively skewness

	![negative](https://segmentfault.com/img/bVbrMw3)

	<br>

####	Divergence, 差异性

*	Q 分位数(有序序列, %)

	Q0 = min, Q1 = 25%, Q2 = 50%(中位数), Q3 = 75%, Q5 = max

	<br>

*	Boxplot 盒子图, 实线盒子的上中下边界是 Q3, Q2, Q1, 虚线上下边界是 max, min

	<br>

*	方差, Variance

	![Variance](https://segmentfault.com/img/bVbrMAC)

	<br>

*	标准差, standard deviation = sqrt(variance)

	<br>

##	可视化, Visualization

*	更加方便直观地体验数据，观察趋势，相关性

	<br>

*	直方图

	不同的颜色 --- 不同类别

	不同的高度，宽度 --- 数量

	<br>

*	散点图

	To describe whether there are relationships, patterns or trends between two numerical variables, 揭示两个变量之间是否存在关联

	正相关，负相关，不相关

	<br>

*	高维数据

	很难同时展示所有维度的数据

	分层展示，树形结构

	<br>

*	非数值型数据

	<br>

	文本：标签，词云，大小颜色

	![word size](https://segmentfault.com/img/bVbrMGs)

	<br>

	图：结点的颜色，大小，边的粗细

	![social network](https://segmentfault.com/img/bVbrMIa)

	<br>

##	距离度量

*	街区距离，曼哈顿距离

*	欧式距离，欧几里得距离

*	闵可夫斯基距离, Minkowski Distance

	![Minkowski Distance](https://segmentfault.com/img/bVbrMJV)

	p = 1 时 变成街区距离, p = 2 时变成欧式距离

*	相似性

	余弦相似性

	<br>