决策树
KNN
Bayes
LR
SVM
NN
DL

k-means
k-medoids

Naive Bayes, Logistic Regression

分类问题之 KNN

<br>

##	简介

*	lazy learning, 训练阶段仅仅保存样本, 没有显式的学习过程, 待收到测试样本后再进行处理

	<br>

*	三要素: k值的选择, 距离度量(街区, 欧式, 闵可夫斯基), 分类决策规则(majority vote)

	<br>

*	分类步骤

	```
	1. 计算测试点到训练集各个点的距离
	2. 找出距离最小的 k 个点
	3. 投票法得出类别
	```

	<br>

*	k 太小, 则对噪声敏感; k 太大, 则可能包含太多其他类别的点

	![k](/img/bVbrQqm)

	<br>

*	属性中需要归一化, 防止某一属性在计算距离的时候占主导地位

	<br>

##	优点

*	复杂度较低

*	效率高

*	非常适用于简单数据集的分类

*	抗噪，对噪声鲁棒

*	选取最优决策树是NP完全问题

	<br>

##	其他

*	PEBLS: Parallel Examplar-Based Learning System (Cost & Salzberg)

	![pebls](/img/bVbrQqI)

	<br>

*	

	<br>

<br>

<br><br>

<br><br>