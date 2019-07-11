分类问题之 Naive Bayes

##	简介

*	条件概率

	```
	P(A|B)*P(B) = P(A, B) = P(B|A)*P(A)
	```

	<br>

*	全概率公式

	![Law_of_total_probability](https://wikimedia.org/api/rest_v1/media/math/render/svg/c7929b8e24c4af9920cf0c17f5793df768d03562)

	<br>

*	分类步骤

	```
	朴素贝叶斯假设特征之间相互独立
	1.计算 P(A|C0) = P(A1, A2, A3|C0) = P(A1|C0) * P(A2|C0) * P(A3|C0)
	2.计算 P(A|C1) = P(A1, A2, A3|C1) = P(A1|C1) * P(A2|C1) * P(A3|C1)
	3.计算 P(A, C0) = P(A|C0) * P(C0)
	4.计算 P(A, C1) = P(A|C1) * P(C1)
	5.不需要计算分母的 P(A)
	6.如果 P(A, C0) > P(A, C1), 那么可以得出 P(C0|A) > P(C1|A), 即样例属于 C0 类
	```

	![clipboard.png](/img/bVbrQsu)

	<br>

##	优缺点

*	抗噪，对孤立噪点噪声鲁棒

*	对不相关的属性有较强的鲁棒性

*	如果某条记录存在缺失值, 之间忽略即可

*	朴素贝叶斯假设特征之间相互独立，但实际情况并不一定如此


*	

	<br>

<br>

<br><br>

<br><br>