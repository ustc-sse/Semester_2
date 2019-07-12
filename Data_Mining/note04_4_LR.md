分类问题之 Logistic Regression

##	线性回归简介

*	线性模型

	![linear model](/img/bVbrQww)

	<br>

*	向量形式

	![vector form](/img/bVbrQwy)

	<br>

*	可解释性

	w 可以理解为特征的权重

	<br>

*	目标

	![target](/img/bVbrQw1)

	<br><br>

##	对数几率回归, Logistic Regression

*	训练过程(梯度下降)

	![train](/img/bVbrQEO)

	![train1](/img/bVbrQEV)

	<br>

*	分类预测步骤

	```
	P(Y=1|x) = e^z / (1 + e^z)
	P(Y=0|x) = 1 / (1 + e^z)
	z = wx
	```

	<br>

##	正则化

![Regularization](/img/bVbrQE2)

<br>

##	与 Naive Bayes 的区别

*	NB 假设特征相互独立

	<br>

*	LR 是线性分类器

	<br>

*	训练方式不一样

	<br>

<br>

<br><br>

<br><br>