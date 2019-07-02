##	高级软件工程四月份课程作业

学号：SAxxxxxxxx

姓名：xxxxxx

<img https://s2.ax1x.com/2019/05/23/VPfCqJ.png alt="题目" style="max-width:100%;width: 540px;">

###	题 2.1

*	图 2.27

	状态转移矩阵

	|    | q0 | q1 | q2 | q3  |
	| -- | -- | -- | -- | --- |
	| q0 |    | 0  | 1  |     |
	| q1 | 1  |    |    | 0   |
	| q2 | 0  |    |    | 1   |
	| q3 |    |    |    | 0,1 |

	状态转移表

	| 输入 | 0  | 1  |
	| ---- | -- | -- |
	| q0   | q1 | q2 |
	| q1   | q3 | q0 |
	| q2   | q0 | q3 |
	| q3   | q3 | q3 |

	<br>

*	图 2.28

	状态转移矩阵

	|   | A | B | C |
	| - | - | - | - |
	| A | 1 | 0 |   |
	| B | 1 |   | 0 |
	| C | 1 |   | 0 |

	状态转移表

	| 输入 | 0 | 1 |
	| ---- | - | - |
	| A    | B | A |
	| B    | C | A |
	| C    | C | A |

	<br>

###	题 12.12

*	应用“逐步求精方法”，为下列一个或多个程序开发三种不同级别的过程抽象：（1）开发一个支票打印程序，给出金额总数，并按支票的常规要求给出大写金额数；（2）为某个超越方程迭代求解；（3）为操作系统开发一个简单的任务调度算法。

	接下来，我会尝试对第（1）个程序应用逐步求精，得到三种不同级别的抽象，由于不太熟悉超越方程和操作系统的知识，所以（2）和（3）暂时省略。

	> Apply a “stepwise refinement approach” to develop three different levels of procedural abstractions for one or more of the following programs: (1) Develop a check writer that, given a numeric dollar amount, will print the amount in words normally required on a check. (2) Iteratively solve for the roots of a transcendental equation. (3) Develop a simple task-scheduling algorithm for an operating system.

####	题目分析

*	首先明确定义，过程抽象是指具有明确和有限功能的指令序列。过程抽象描述了功能，但隐藏了具体的细节。过程抽象在书上的例子是【开门】，其中【开】隐含了一长串的过程步骤（如走到门前，伸出手，抓住把手，转动把手，拉门等）。

	而逐步求精，是一种自顶向下的设计策略，通过连续细化过程细节进行应用开发，通过逐步分解功能的宏观描述（过程抽象）进行层次开发，直到最终完成程序设计语言的语句。最初的功能陈述或者信息描述只是概念性地描述了功能或信息，但是没有提供内部的工作细节和结构。每一次细化都将提供越来越多的细节。

*	因此，对于【（1）开发一个支票打印程序，给出金额总数，并按支票的常规要求给出大写金额数】，三种不同级别的过程抽象如下：

	注: 以如下支票模板为例

	![招行支票](https://s2.ax1x.com/2019/05/25/VkVTaT.jpg)

####	第一步求精

*	将整个流程分成三大块

	```
	a). 获取用户输入
	b). 处理数据
	c). 输出
	```

	<br>

####	第二步求精

*	对于 `a). 输入`, 可以细化成如下过程

	```
	a1). 输入阿拉伯数字金额
	a2). 输入开票的日期
	a3). 输入付款人的开户行
	a4). 输入付款人姓名
	a5). 输入付款人账号
	a6). 输入收款人姓名
	a7). 输入收款人账号
	a8). 输入备注, 如用途
	```

	对于 `b). 处理数据`, 可以细化为如下内容

	```
	b1). 根据输入的阿拉伯数字金额，得到大写金额数
	```

	对于 `c). 输出`, 可以细化为如下内容

	```
	c1). 读取支票模板，准备进行格式化的字符串替换
	c2). 在金额处填写阿拉伯数字和大写数字
	c3). 在日期处填写日期
	c4). 在付款人处填写付款人姓名和账号
	c5). 在收款人处填写收款人姓名和账号
	c6). 在备注处填写备注
	c7). 打印输出
	```

	由此可以得到如下结构图和UML类图

	![12.12](https://s2.ax1x.com/2019/05/25/VkV0KI.png)

	<br>

####	第三步求精

*	对于 `a). 输入`, 可以得到如下 Java 类

	```java
	class MyInput{
		public MyInput(MyCheck check){
			readAmount(check);
			readDate(check);
			readBank(check);
			readTx(check);
			readRx(check);
			readRemark(check);
		}

		private void readAmount(MyCheck check){
			System.out.println("请输入金额");
			Scanner in = new Scanner(System.in);
			int amount = Integer.parseInt(in.nextLine());
			check.setDigitAmount(amount);
		}

		private void readDate(MyCheck check){
			System.out.println("请输入日期");
			Scanner in = new Scanner(System.in);
			String date = in.nextLine();
			check.setDate(date);
		}

		private void readBank(MyCheck check){
			System.out.println("请输入开户行");
			Scanner in = new Scanner(System.in);
			String bank = in.nextLine();
			check.setBank(bank);
		}

		private void readTx(MyCheck check){
			System.out.println("请输入付款人姓名");
			Scanner in = new Scanner(System.in);
			String txname = in.nextLine();
			check.setTxName(txname);

			System.out.println("请输入付款人账号");
			String txid = in.nextLine();
			check.setTxId(txid);
		}

		private void readRx(MyCheck check){
			System.out.println("请输入收款人姓名");
			Scanner in = new Scanner(System.in);
			String rxname = in.nextLine();
			check.setRxName(rxname);

			System.out.println("请输入收款人账号");
			String rxid = in.nextLine();
			check.setRxId(rxid);
		}

		private void readRemark(MyCheck check){
			System.out.println("请输入备注");
			Scanner in = new Scanner(System.in);
			String remark = in.nextLine();
			check.setRemark(remark);
		}
	}
	```

	对于 `b). 处理数据`, 可以得到如下 Java 类

	```java
	class MyCheck{
		private int mdigitAmount;
		String mwordsAmount;
		String mdate;
		String mbank;
		String mtxName, mtxId;
		String mrxName, mrxId;
		String mremark;

		MyCheck(){}

		private String getWordsFromDigitAmount(int amount){
			String res = "";
			// TODO
			return res;
		}

		public void setDigitAmount(int amount){
			mdigitAmount = amount;
			mwordsAmount = getWordsFromDigitAmount(amount);
		}

		public int getDigitAmount(){
			return mdigitAmount;
		}

		public String getWordsAmount(){
			return mwordsAmount;
		}

		public void setTxName(String txname){
			mtxName = txname;
		}

		public String getTxName(){
			return mtxName;
		}

		// 其他 setter() 和 getter()
	}
	```

	对于 `c). 输出`, 可以得到如下 Java 类

	```java
	class MyOutput{
		public MyOutput(MyCheck check){
			Map<String, String> map = getMap(check);

			String template = readTemplate();
			String content = StringUtil.replace(template, map);

			printContent(content);
		}

		private Map<String, String> getMap(MyCheck check){
			Map<String, String> map = new HashMap<String, String>();
			map.put("digitAmount",  String.valueOf(check.getDigitAmount()));
			map.put("wordsAmount", check.getWordsAmount());
			map.put("date", check.getDate());
			map.put("txname", check.getTxName());
			map.put("txid", check.getTxId());
			map.put("rxname", check.getRxName());
			map.put("rxid", check.getRxId());
			map.put("remark", check.getRemark());
			return map;
		}

		private String readTemplate(){
			String template = "...${digitAmount}...${wordsAmount}...${date}.....";
			return template;
		}

		private void printContent(String content){
			System.out.println(content);
		}
	}
	```

	<br>

###	题 12.13

*	考虑需要实现汽车导航功能（GPS），手持通信设备的软件。描述两个或三个要表示的横切关注点。讨论如何将其中一个关注点作为方面来表示。

	> Consider the software required to implement a full navigation capability (using GPS) in a mobile, handheld communication device. Describe two or three crosscutting concerns that would be present. Discuss how you would represent one of these concerns as an aspect.

####	题目分析

*	关注点，包含了需求、用例、特征、数据结构、服务质量、知识产权边界、协作等内容

*	A横切B，也就是B可以被分成两块，A插在B中间，比如书上的例子，B表示注册用户可以访问摄像头，A表示注册用户必须先经过认证才能使用 webapp

*	根据[维基百科](https://zh.wikipedia.org/wiki/%E9%9D%A2%E5%90%91%E5%88%87%E9%9D%A2%E7%9A%84%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1), 面向切面的程序设计（Aspect-oriented programming，AOP，又译作面向方面的程序设计、剖面导向程序设计）是计算机科学中的一种程序设计思想，旨在将横切关注点与业务主体进行进一步分离，以提高程序代码的模块化程度。

*	因此，对于本题，可以得到如下的几个横切关注点

####	横切关注点

*	横切关注点 1

	在使用导航设备之前必须进行身份验证，原因同教材

*	横切关注点 2

	在使用导航设备的过程中，需要记录运行日志，记录日志的功能需求是常见的横切关注点

*	另外，由于最近在学习 Python, 个人感觉其中的装饰器 decorator, 很像是一种实现横切关注点的方法, 

	<br>

####	参考资料

*	[逐步求精](https://blog.csdn.net/xyb890826/article/details/41728475)

*	[支票怎么写](https://www.guruin.com/articles/2044)

*	[百度百科, 支票](https://baike.baidu.com/item/%E6%94%AF%E7%A5%A8)

*	[Java字符串模板格式化](https://my.oschina.net/venusdrogon/blog/1486633)

*	[使用面向 Aspect 的编程改进模块性](https://www.ibm.com/developerworks/cn/java/j-aspectj/index.html)

*	[维基百科, 面向切面的程序设计](https://zh.wikipedia.org/wiki/面向切面的程序设计)

*	[维基百科, 软件工程](https://zh.wikipedia.org/wiki/软件工程)

*	[Aspect面向方面编程](https://www.cnblogs.com/lori/p/8422670.html)

*	[Python装饰器](https://www.cnblogs.com/youxin/p/3383059.html)

*	[理解Python装饰器](https://foofish.net/decorator.html)