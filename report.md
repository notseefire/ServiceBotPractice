# **一种领域特定脚本语言的解释器的设计与实现**

# 

## 内容

> 领域特定语言（Domain Specific Language，DSL）可以提供一种相对简单的文法，用于特定领域的业务流程定制。本作业要求定义一个领域特定脚本语言，这个语言能够描述在线客服机器人（机器人客服是目前提升客服效率的重要技术，在银行、通信和商务等领域的复杂信息系统中有广泛的应用）的自动应答逻辑，并设计实现一个解释器解释执行这个脚本，可以根据用户的不同输入，根据脚本的逻辑设计给出相应的应答。 

​	本项目使用 ` C++ 17 `  语言实现一个在线客服机器人服务器，并且使用Python QQ机器人框架实现，使用户可以通过QQ与客服机器人进行人机交互。



## 项目管理

​	本项目使用 `cmake 3.21` 进行管理，使用到的第三方库分别有 `jsonxx` ， `boost.asio` , `Boost` .

​	单元测试使用 [google test](https://github.com/google/googletest/) 搭配 `cmake `目录进行管理 

​	Code Style 可参考 C++ Google Code Style 和 [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines). 	

​	在 `cmake` 中，可以通过 `add_subdirectory` 添加子项目目录， 此时 `cmake` 会同时链接子项目目录下的 `CMakeLists.txt`文件，从而实现树形的管理方法.

​	

## DSL 定义 与  `VituralMachine` 操作定义

​	假设你是一个用户小明，你从U公司购买了一批T病毒，之后你想向U公司询问一些使用事项，在使用T病毒后，发现病毒已经失活，无法进行生物实验，你可能还需要向U公司发起一些退款申请。

​	这些申请我们可以总结为工单，客户在遇到一些在机器人处无法解决的问题时，需要发起工单，工单会保存在数据库，等待人工客户进行处理

​	使用事项可以总结为，当客服机器人接收到一些请求时，需要做出响应的回复.

​	最后客服机器人在开启和结束时，需要特殊定义一些类似于问候语和结束语的回复

​	并且可以确定的是，大部分客服机器人都具有分支功能，因为要提供状态机的功能

所以在本项目设计的关键字及语法功能有

```shell

echo {expression} 回复消息
set {id} [= string] 设置变量名及变量值
input {id} [wait {number}] 等待用户输入消息，并将消息内容储存在变量中
assign {id} = {expression} 修改变量的值
call {id} = {string} {proc_id} 调用过程proc_id
loop endloop 循环体
break 结束循环的运行
其中，expression指的是这样的语法结构
expression = {id|string} [..[+ {id|string}]]
```

使用这套`DSL` 实现U公司的需求可以使用以下脚本

```shell
proc main:
	set greet
	input greet

	echo "输入启动开始服务"
	call greet = "启动" begin

proc begin:
	echo "帮助:
  输入退款启动退款服务
  输入工单购买商品
  输入退出可关闭服务"
	set function
	loop
		input function wait 30
		call function = "退款" refund
		call function = "工单" order
		call function = "" silence
		call function = "退出" out
		other wrong
	endloop
	echo "再见"

proc out:
	echo "系统将退出"

proc silence:
	echo "没有接收到输入，将自动退出"

proc wrong:
	echo "输入了未知的功能，请重新输入"
	
proc refund:
	set item
	set number
	echo "请输入退款的商品"
	input item
	echo "请输入退款商品的数量"
	input number
	echo "记录:退款" + number + "个" + item 
	echo "请等待工作人员核实"
	
proc order:
	set item
	set number
	echo "输入购买的商品"
	input item
	echo "输入购买商品的数量"
	input number
	assign number = number + "个" + item
	echo "记录:购买" + number 
	echo "请等待工作人员核实"
```

## 顶层设计

本项目设计的模块如下

![image-20211128231550028](report.assets/image-20211128231550028.png)

### Parser 

Parser的功能为解释设计的DSL语言，生成可执行代码，符号表。



### Web 

​	实现了请求与响应`Mirai`服务器的功能。使用 `cpp-httplib` 库实现，同时使用 `jsonxx` 库进行 `json` 的逆序列化。

​	实现了 `Runtime` 子模块，该模块具有运行`Parser `编译完成后`AST`的能力



### Log

​	日志功能，该模块主要在 `CLI` 环境下交互，以及产生日志文件

​	日志信息包括：`Paser` 的编译错误信息， `Runtime` 的日志信息，使用了 `Boost` 的`log`组件实现.



### 外部Web框架

​	`Mirai` 是一个第三方程序，该模块使用了 `Mirai` 框架提供的 Web 服务器， 该 Web 服务器实现了使用设备配置文件登陆 QQ ，可以向POST上报地址发送 `Json` 格式的消息



## 详细设计

### Parser

#### Token



​	根据之前设计的Notation，接下来要利用已有的Compiler Principle的知识，设计解释器。首先是Token的设计，我们可以将 Token 分为`string` ，`identifier`, `reserved_token`，`int`。

​	`reserved_token` 表示Parser保留的关键字，可以通过在 `lookup_table` 中查询得到

​	`lookup_table` 的类型是 `map<string, reserved_token>` ，在



### Web设计与多线程实践

首先将QQ与主线程的访问关系梳理出来

 ![image-20211204173719837](report.assets/image-20211204173719837.png)

可以看出，主线程负责执行的任务包括:

- 监听 QQ 发来的消息
- 管理发送消息到线程的消息队列
- 检查线程的状态，创建线程

子线程的任务有：

- 执行用户自定义的脚本
- 访问消息队列
- 发送QQ消息

其中不难发现

检查线程状态与线程退出，访问消息队列是互斥的，为了避免竞争，需要加锁

#### Modern C++ Mutex

为了实现线程间的同步与通信，我们需要使用 C++ 提供的 `std::mutex` 。

在该项目中，需要使用到 `std::mutex` 来实现 QQ消息队列，线程管理，释放队列

在使用`mutex`时，C++提供`std::lock_guard<std::mutex>`和`std::unique_lock<std::mutex>`方法，前者可以方便向`mutex`申请锁，后者则是申请一个`movable`的锁，可以从函数中返回

#### 线程管理

线程管理功能主要包含线程的创建以及线程的退出

相关的数据结构为

```cpp

std::map<qq_id, Parallel*> thread_pool;
std::mutex free_mutex;
std::queue<Parallel *> free_queue;
std::mutex thread_pool_mutex;
class QMessageQueue
```

​	`Parallel` 近似于一个进程，当实例化一个`Parallel`时，`Parallel`会自动创建一个线程，该线程可以使用`Parallel`提供的资源运行`AST`



​	`thread_pool` 是检测`qq_id`是否有对应线程的数据结构，当一个`qq_id`已经在线程中时，就不需要创建新的线程，而是将消息放入对应线程的消息队列中。其他线程与主线程对`thread_pool`是互斥访问的，需要使用`thread_pool_mutex`管理互斥访问

​	`free_queue` 指的是需要被释放`Parallel`的队列，当`Parallel`结束`AST`的执行后，会执行将自己从`thread_pool`中删除的命令，同时将自己放入到释放队列中。控制释放队列的是一个主线程在初始化时创建的线程，该线程每隔50ms会检查一次释放队列中是否有新的元素，如果有则全部释放。 所以释放线程与其他线程是互斥访问的，所以还需要添加`free_mutex`进行进程间的同步。	

​	`Parallel`管理的资源为 `std::thread` `qq_id` `QMessageQueue`  `Runtime` `statments_table` 

​	`QMessageQueue`实现了管理消息的最小单位，显示地提供了队列方法

```cpp
std::optional<QMessage> pop(int micro_seconds);
void push(QMessage msg);
```

`pop`中封装了 `wait_data` 方法，当等待 `micro_seconds`后消息队列仍然为空，该方法返回`std::nullopt`

`wait_data`中同时封装了互斥锁和等待循环, `wait_data` 每100ms检查一次队列，注意等待期间锁是被释放的。

`push`中封装了`get_lock`方法，调用该方法会自动向`_mutex`申请锁，然后将消息放入到队列中

#### Runtime

实现`Runtime`需要额外两种结构`Context`和`AstStatment`

```cpp
 virtual void run(Runtime* runtime, Parallel* thread) = 0;
 void set_target(size_t target);
 size_t get_target();
```

`AstStatment`封装了执行代码的接口，所有可执行代码都需要实现这个虚类接口。

`run`是运行`Ast`的最小单位，包含的功能仅限于执行一个指令，并且提供了执行指令时需要的`Runtime`和`Parallel`信息，`Runtime`负责获取当前的`Context`，`Parallel`负责服务器需要的系统调用

`set_target`和`get_target`则提供了实现跳转指令时有用的信息，`target`的设置在`Parser`阶段就应该完成

`Context`封装了一个函数中的符号表作用域，并且具有指向代码段的指针，和保存当前代码运行到的区域。在调用函数成功时创建`Context`

`Runtime`封装了当前运行到`Context`和`Context`栈，全局代码段，即不同函数的代码段，用于创建`Context`实例，当运行完一个`Context`时，`Runtime`会弹出当前运行到的`Context`并且装载下一个`Context`













