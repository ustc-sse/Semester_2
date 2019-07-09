决策树
KNN
Bayes
LR
SVM
NN
DL

k-means
k-medoids


分类问题之决策树

<br>

##	优点

*	复杂度较低

*	效率高

*	非常适用于简单数据集的分类

*	抗噪，对噪声鲁棒

*	选取最优决策树是NP完全问题

	<br>

##	划分选择

*	使结点纯度越来越高

*	信息熵

	Ent(D) = -Σ (Pk)·log(Pk)

*	信息增益, Gain

	Gain(D, a) = Ent(D) - Σ (Di/D)·Ent(Di)

	Ent(Di) = -Σ (Pk)·log(Pk)

	ID3 算法使用 Gain 进行划分, 选择信息增益最大的作为划分特征

	缺点：倾向于选择取值更多的特征

	例题

	![ML1](/img/bVbrQlC)

	![ML2](/img/bVbrQl2)

	![ML3](/img/bVbrQl5)

	![ML4](/img/bVbrQl6)


	<br>

*	信息增益率, GainRatio

	GainRatio(D, a) = Gain(D, a) / IV(a),

	IV(a) = -Σ (Di/D)log(Di/D), 属性的熵, 属性的固有值, 如果某一特征取值过多，那么它的 IV 就会很大，会被惩罚

	C4.5 算法使用 GainRatio 进行划分, 在信息增益 Gain 高于 Avg 的属性中，选择 GainRatio 最大的属性作为划分属性

	例题(对应上一小节的数据集)

	```
	IV(编号) = -Σ (1/17)·log(1/17) = -log(1/17) = 4.088
	IV(触感) = - [ (5/17)log(5/17) + (12/17)log(12/17) ] = 0.874
	```

	<br>

*	基尼指数, Gini

	Gini(D) = 1 - Σp^2

	二路划分时（二叉树）, Gini(D) = 2p(1 - p)

	多路划分时, Gini(D) = Σ (Di/D)·Gini(Di)

	CART 算法使用 Gini 指数进行划分, 选择 Gini 指数最小的属性作为划分属性

	注: 多路划分肯定比二路划分更纯，Gini也更小

	例题

	![Gini1](/img/bVbrQjJ)

	![Gini2](/img/bVbrQkv)

	```
	0.393 = (5/10)·(2·0.2·0.8) + (3/10)·(2·0.67·0.33) + (2/10)·(2·0.5·0.5)
	```

	<br>

*	分类误差率

	Error(t) = 1 - max P(i|t)

	![c_error](/img/bVbrQd9)

	<br>

*	Hunt(略)

	<br>

##	何时停止

*	同一结点中所有的记录都属于同一类

*	同一结点所有属性值相近

*	早停(剪枝)

	<br>


*	

	<br>

*	

	<br>

*	

	<br>

*	

	<br>

<br>

<br><br>

<br><br>