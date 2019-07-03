##	高级软件工程五月份课程作业

学号：SAxxxxxxxx

姓名：xxxxxx

##	题目

1.1 除了教材提到的体系结构风格, 还有哪些体系结构风格？

1.2 请举一个体系结构的实际应用案例。

<br>

2.1 熟悉集合管理器、容器、自容类、关系环、二元关联类、手柄本体（桥）模式、动态模式。

2.2 何为设计模式？请说明简单工厂的含义, 并画出其结构, 用你最熟悉的面向对象语言给出其代码框架。

2.3 何为设计模式？请说明工厂方法的含义, 并画出其结构, 用你最熟悉的面向对象语言给出其代码框架。

2.4 何为设计模式？请说明装饰者模式（Decorator）的含义, 并画出其结构, 用你最熟悉的面向对象语言给出其代码框架。

<br>

3.1 将《面向行为的设计（软件工程JAVA语言实现第11章）》的图 11-6 的 4 个模块的详细设计用伪代码的形式表现出来。

![hw03-3-1](https://s2.ax1x.com/2019/06/17/VHAWPf.png)

3.2 将《面向行为的设计（软件工程JAVA语言实现第11章）》的程序清单 11-1 的 4 个模块的伪代码用详细设计的形式表现出来。

![hw03-3-2](https://s2.ax1x.com/2019/06/17/VHAbaq.md.png)

<br><br><br><br>

##	解答

###	题 1.1

*	教材( `20190505-09/PDF/第13章体系结构设计.pdf, P189, 13.3.1` )提到了以数据为中心的体系结构, 数据流体系结构, 调用和返回体系结构, 面向对象体系结构, 层次体系结构

	此外，还有虚拟机风格, 仓库风格, C/S(客户/服务器)风格等体系结构风格。

*	注：根据 [软件工程：实践者的研究方法（原书第8版）](https://book.douban.com/subject/26918148/) 和 [【软件工程学习笔记】软件体系结构风格](https://zhuanlan.zhihu.com/p/39784838)

	> 软件体系结构指系统的一个或多个结构, 它包括软件构件、构件的外部可见属性以及它们之间的相互关系。它并非可运行的软件, 而是一种表达, 使我们能对设计进行有效性分析, 选择解决方案, 降低风险。设计是体系结构的一个实例。比如 `Java EE` 或者 `Microsoft .Net Framework` 就是两种体系结构。
	>
	> 软件体系结构风格（Architectural Styles）是描述特定系统组织方式的惯用范例, 强调了软件系统中通用的组织结构。

	<br>

###	题 1.2

*	`C/S` 风格的实例：用户上网时访问的各种网站都是基于 `C/S` 体系结构风格

	<br><br>

###	题 2.1

*	集合管理器

	用一个 管理器( `Manager` ) 来管理一组对象(对象集合), 包括构造、增加、编辑、去除集合中的对象。

	比如有一个 `User` 类的 n 个对象, 我们不是直接把它们放到数组里面增删查改, 这样会暴露太多细节, 不便于维护。集合管理器的设计模式是, 另外写一个 `UserManager`, 外部想要构造新的 `User`, 只需要给 `UserManager` 一个 `username`, 如 `um.add(username)`, 而不需要手动构造 `User`, 增删查改也只需要 `username` 这么一个参数, 而不需要手动去遍历集合。

*	容器

	容器( `Container` )和集合管理器类似, 但是容器只存放对象而不构造对象。对比上面的 `um.add(username)`, 容器则是 `c.add(new User(username))`

*	自容类

	一个对象实例可以和另一个同类的对象实例有链接。就像链表, 一个结点有数据域也有指针域, 可以指向另一个同类结点。如果一个容器对象可以包含其他同类的容器对象, 那么这是一个自容类。

*	关系环

	不同类之间的复杂关系可能会导致递归关联, 比如 `A` 包含多个 `B`, `B` 也包含多个 `A`, 导致 `AB` 之间形成一个环, 当两者存在聚合和继承关系时, 就形成了一个 `关系环`

*	二元关联类

	两个对象之间的连接( `link` )本身也可能拥有自己的属性和方法。比如一个交易是买方和卖方的 `link` , 这个 交易link 可以有名称、数量、价格、时间等属性。如果把这个 link 看出一个对象实例, 就是关联类, 如果这个 `link` 只连接两个类, 那就是二元关联类

*	手柄本体(handle-body)模式 / 桥模式

	根据 [秒懂设计模式之桥接模式（Bridge Pattern）](https://zhuanlan.zhihu.com/p/58903776) 和 [java设计模式之桥接模式](https://www.cnblogs.com/lfxiao/p/6815760.html), 桥接模式是一种很实用的结构型设计模式, 如果软件系统中某个类存在两个独立变化的维度, 通过该模式可以将这两个维度分离出来, 使两者可以独立扩展, 让系统更加符合“单一职责原则”。与多层继承方案不同, 它将两个独立变化的维度设计为两个独立的继承等级结构, 并且在抽象层建立一个抽象关联, 该关联关系类似一条连接两个独立继承结构的桥, 故名桥接模式。

*	动态模式(dynamic schema)

	可以随时把对象进行分类, 而无须改变设计或重新编译源码。动态模式使对象可以为客户提供广泛且可扩展的属性集合。模式中的性质可以随时增加、减少或改变, 十分灵活, 所以叫动态模式。

	<br>

###	题 2.2

*	设计模式（Design pattern）代表了最佳的实践，通常被有经验的面向对象的软件开发人员所采用。设计模式是软件开发人员在软件开发过程中面临的一般问题的解决方案。这些解决方案是众多软件开发人员经过相当长的一段时间的试验和错误总结出来的。

	软件模式是将模式的一般概念应用于软件开发领域，即软件开发的 总体指导思路或参照样板。软件模式并非仅限于设计模式，还包括 架构模式、分析模式和过程模式等，实际上，在软件生存期的每一 个阶段都存在着一些被认同的模式。

	个人感觉, 设计模式还有点像数据结构。通常来说, 一个问题包含多个对象, 而多个对象之间是有关联的, 设计模式就是怎么组织对象之间的关系, 使得我们可以更好地分析问题, 重用代码/解决方案。

*	简单工厂的含义

	简单工厂根据参数的不同返回不同的对象实例，被创建的对象实例通常都具有共同的父类。简单工厂模式使其创建过程延迟到子类进行。

*	画出结构

	![simple factory](https://s2.ax1x.com/2019/06/22/Z9fB4g.png)

*	代码框架

	```java
	// 1. 创建一个接口
	interface Shape{
		void draw();
	}

	// 2. 实现接口
	// 画矩形
	class Rectangle implements Shape{
		@Override
		public void draw(){
			System.out.println("draw a rectangle");
		}
	}
	// 画圆形
	class Circle implements Shape{
		@Override
		public void draw(){
			System.out.println("draw a circle");
		}
	}

	// 3. 创建工程, 基于参数信息，生成并返回实例对象
	class ShapeFactory{
		public Shape getShape(String shapeType){
			if( shapeType == null ){
				return null;
			}
			else if( shapeType.equalsIgnoreCase("Circle") ){
				return new Circle();
			}
			else if( shapeType.equalsIgnoreCase("Rectangle") ){
				return new Rectangle();
			}
			else{
				return null;
			}
		}
	}
	// 4. 使用测试
	public class Main{
		public static void main(String args[]) {
			ShapeFactory factory = new ShapeFactory();
			try{
				Shape shape;
				shape = factory.getShape("circle");
				shape.draw();
				shape = factory.getShape("rectangle");
				shape.draw();
				shape = factory.getShape("biubiubiu");
				shape.draw();
			}
			catch( NullPointerException e ){
				System.out.println("没有这种形状");
			}
		}
	}
	```

	<br>

###	题 2.3

*	工厂方法的含义

	工厂方法模式，又称工厂模式、多态工厂模式和虚拟构造器模式，通过定义工厂父类负责定义创建对象的公共接口，而子类则负责生成具体的对象。

	简单工厂中，工厂一旦需要生产新产品就需要修改工厂类的方法逻辑，违背了“开放 - 关闭原则。工厂方法就是为了解决简单工厂的这个缺点。

	在工厂方法模式中，对于抽象工厂类只需要提供一个创建产品的接口，而由其子类来确定具体要创建的对象，利用面向对象的多态性，在程序运行时，子类对象将覆盖父类对象，从而使得系统更容易扩展。

	缺点是，添加新产品时，除了增加新产品类外，还要提供与之对应的具体工厂类，系统类的个数将成对增加，在一定程度上增加了系统的复杂度；同时，有更多的类需要编译和运行，会给系统带来一些额外的开销；

*	画出结构

	![factory method](https://s2.ax1x.com/2019/06/19/VXjPD1.md.png)

*	代码框架

	```java
	// 1. 产品接口
	interface Shape{
		void draw();
	}
	// 矩形产品
	class Rectangle implements Shape{
		@Override
		public void draw(){
			System.out.println("draw a rectangle");
		}
	}
	// 圆形产品
	class Circle implements Shape{
		@Override
		public void draw(){
			System.out.println("draw a circle");
		}
	}

	// 2. 抽象工厂
	interface ShapeFactory{
		public Shape getShape();
	}
	// Rectangle 的工厂
	class RectangleFactory implements ShapeFactory{
		@Override
		public Shape getShape(){
			return new Rectangle();
		}
	}
	// Circle 的工厂
	class CircleFactory implements ShapeFactory{
		@Override
		public Shape getShape(){
			return new Circle();
		}
	}

	// 3. 使用测试
	public class Main{
		public static void main(String args[]) {
			ShapeFactory factory;
			factory = new RectangleFactory();
			factory.getShape().draw();
			factory = new CircleFactory();
			factory.getShape().draw();
		}
	}
	```

	<br>

###	题 2.4

*	装饰者模式 Decorator 的含义

	装饰器模式（Decorator Pattern）允许向一个现有的对象添加新的功能，同时又不改变其结构。这种类型的设计模式属于结构型模式，它是作为现有的类的一个包装。

	这种模式创建了一个装饰类，用来包装原有的类，并在保持类方法签名完整性的前提下，提供了额外的功能。

*	画出结构

	![decorator](https://s2.ax1x.com/2019/06/22/Z9fV39.png)

*	代码框架

	```java
	// 形状接口
	interface Shape{
		public void draw();
	}
	// 形状实现，矩形
	class Rectangle implements Shape{
		@Override
		public void draw(){
			System.out.println("Rectangle");
		}
	}
	// 形状实现，圆形
	class Circle implements Shape{
		@Override
		public void draw(){
			System.out.println("Circle");
		}
	}

	// 装饰器
	abstract class ShapeDecorator implements Shape{
		Shape shape;

		public ShapeDecorator(Shape shape){
			this.shape = shape;
		}

		@Override
		public void draw(){
			shape.draw();
		}
	}
	// 加红色边框的装饰器
	class RedBorderDecorator extends ShapeDecorator{
		public RedBorderDecorator(Shape shape){
			super(shape);
		}

		private void setRedBorder(){
			System.out.println("The border is now red");
		}

		@Override
		public void draw(){
			shape.draw();
			setRedBorder();
		}
	}

	// 测试
	public class Main{
		public static void main(String args[]) {
			Shape circle = new Circle();
			circle.draw();
			System.out.println();
			ShapeDecorator redBorder = new RedBorderDecorator(circle);
			redBorder.draw();
		}
	}
	```

	两种形状，一种边框，如果为两种形状的每种边框各自单独写一个类（假设颜色混合后将产生新的颜色），一共要写 `2 * 2^1 = 4` 个类，用装饰器模式，只需要 `5` 个类。

	然而，当边框类型更多，比如有 `4` 种边框，那么为两种形状的四种边框各自单独编写一个类，一共要写 `2 * 2^4 = 32` 个类，而装饰器模式只需要 `8` 个类

	<br><br>

###	题 3.1

*	模块名 `read file name`

	```java
	String read file name(){
		String filename = "";
		String[] inputStringList = System call readline.split(" ");
		if( inputStringList.length > 1 ){
			filename = inputStringList[1];
		}
		return filename;
	}
	```

*	模块名 `validate file name`

	```java
	boolean validate file name(String filename){
		if( find filename using system call ){
			return true;
		}
		else{
			return false;
		}
	}
	```

*	模块名 `count number of words`

	```java
	int count number of words(String validatedFilename){
		InputStream fp = new FileInputStream(validatedFilename);
		int cnt = 0;
		if( fp is text file ){
			String text = read text file from fp;
			for( word in text ){
				cnt ++;
			}
		}
		else{
			cnt = -1;
		}
		return cnt;
	}
	```

*	模块名 `produce output`

	```java
	void produce output(int wordCount){
		display word count( format word count(wordCount) );
	}
	```

	<br>

###	题 3.2

*	伪代码名 `perform word count`

	| 条目 | 描述 |
	| -- | -- |
	| 模块名 | perform word count(执行单词计数) |
	| 模块返回 | 无 |
	| 输入参数 | 无 |
	| 输出参数 | 当文件名不存在时, 输出 "error 1: file does not exists" |
	|          | 当 count 是 -1 时输出 "error 2: file is not a text file" |
	| 出错消息 | 无 |
	| 访问的文件 | 无 |
	| 修改的文件 | 无 |
	| 调用模块 | get input |
	|          | 参数：validated file name : String |
	|          |  |
	|          | count number of words |
	|          | 参数：validated file name |
	|          |  |
	|          | produce output |
	|          | 参数：word count |
	|          |  |
	| 注释 | 首先读取 filename, 然后判断对应的文件是否存在 |
	|      | 如果 filename 不存在, 则 |
	|      | 　　输出 "error 1: file does not exists" |
	|      | 否则 |
	|      | 　　调用 count number of words, 得到 word count |
	|      | 　　如果 word count 等于 -1, 则 |
	|      | 　　　　输出 "error 2: file is not a text file" |
	|      | 　　否则 |
	|      | 　　　　调用 produce output |

*	伪代码名 `get input`

	| 条目 | 描述 |
	| -- | -- |
	| 模块名 | get input(获取输入) |
	| 模块返回 | isValid : boolean |
	| 输入参数 | validated file name : String |
	| 输出参数 | 无 |
	| 出错消息 | 无 |
	| 访问的文件 | 无 |
	| 修改的文件 | 无 |
	| 调用模块 | validate file name |
	|          | 参数：file name : String |
	|          |  |
	| 注释 | 首先读取 file name, 然后判断对应的文件是否存在 |
	|      | 如果 file name 存在, 则 |
	|      | 　　将 file name 赋值给 validated file name  |
	|      | 　　返回 true |
	|      | 否则 |
	|      | 　　返回 false |

*	伪代码名 `display word count`

	| 条目 | 描述 |
	| -- | -- |
	| 模块名 | display word count(显示单词计数) |
	| 模块返回 | 无 |
	| 输入参数 | formatted word count : String |
	| 输出参数 | 无 |
	| 出错消息 | 无 |
	| 访问的文件 | 无 |
	| 修改的文件 | 无 |
	| 调用模块 | 无 |
	|          |  |
	| 注释 | 通过系统调用, 将格式化的单词计数打印出来 |

*	伪代码名 `format word count`

	| 条目 | 描述 |
	| -- | -- |
	| 模块名 | format word count(格式化单词计数) |
	| 模块返回 | formatted word count : String |
	| 输入参数 | word count : int |
	| 输出参数 | 无 |
	| 出错消息 | 无 |
	| 访问的文件 | 无 |
	| 修改的文件 | 无 |
	| 调用模块 | 无 |
	|          |  |
	| 注释 | 返回字符串 "File contains %d words" % word count |

	<br><br>

####	参考资料

*	[【软件工程学习笔记】软件体系结构风格](https://zhuanlan.zhihu.com/p/39784838)

*	[秒懂设计模式之桥接模式（Bridge Pattern）](https://zhuanlan.zhihu.com/p/58903776)

*	[java设计模式之桥接模式](https://www.cnblogs.com/lfxiao/p/6815760.html)

*	[设计模式](https://www.runoob.com/design-pattern/design-pattern-tutorial.html)

*	[简单工厂模式( Simple Factory Pattern )](https://design-patterns.readthedocs.io/zh_CN/latest/creational_patterns/simple_factory.html)

*	[看懂UML类图和时序图](https://design-patterns.readthedocs.io/zh_CN/latest/read_uml.html)

*	[深入浅出UML类图](http://www.uml.org.cn/oobject/201211231.asp)

*	[工厂方法模式（Factory Method）- 最易懂的设计模式解析](https://www.jianshu.com/p/d0c444275827)

*	[设计模式——装饰模式（Decorator）](https://blog.csdn.net/zhshulin/article/details/38665187)
