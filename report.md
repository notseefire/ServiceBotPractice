# **一种领域特定脚本语言的解释器的设计与实现**

# 

## 内容

> 领域特定语言（Domain Specific Language，DSL）可以提供一种相对简单的文法，用于特定领域的业务流程定制。本作业要求定义一个领域特定脚本语言，这个语言能够描述在线客服机器人（机器人客服是目前提升客服效率的重要技术，在银行、通信和商务等领域的复杂信息系统中有广泛的应用）的自动应答逻辑，并设计实现一个解释器解释执行这个脚本，可以根据用户的不同输入，根据脚本的逻辑设计给出相应的应答。 

​	本项目使用 ` C++ ` 语言实现一个在线客服机器人服务器，并且使用Python QQ机器人框架实现，使用户可以通过QQ与客服机器人进行人机交互。





## DSL 定义 与  `VituralMachine` 操作定义

​	假设你是一个用户小明，你从U公司购买了一批T病毒，之后你想向U公司询问一些使用事项，在使用T病毒后，发现病毒已经失活，无法进行生物实验，你可能还需要向U公司发起一些退款申请。

​	这些申请我们可以总结为工单，客户在遇到一些在机器人处无法解决的问题时，需要发起工单，工单会保存在数据库，等待人工客户进行处理

​	使用事项可以总结为，当客服机器人接收到一些请求时，需要做出响应的回复.

​	最后客服机器人在开启和结束时，需要特殊定义一些类似于问候语和结束语的回复

​	并且可以确定的是，大部分客服机器人都具有分支功能，因为要提供状态机的功能

所以在本项目设计的关键字及语法功能有

```shell
#include [script] 预处理，表示程序需要引用到

echo [string] 回复消息
set [id] = [value] 设置变量名及变量值
input [id] 等待用户输入消息，并将消息内容储存在变量中 
call [script] 调用另外一份脚本文件 script

条件分支语句
if [condition]; then
	[code]
elif [condition]; then
	[code]
else then
	[code]
fi

loop condition; do 循环体开始
done 循环体结束
break 结束循环
configure 

```

使用这套`DSL` 实现U公司的需求可以使用以下脚本

```shell
/main.u
#include process

loop true; do
	input hello
    if hello == "开始"; then
    	call process
    fi
done

/process.u
#include refund
#include question

echo "hello, this is Umbrella Copration"
echo 'please enter ":help" to get manual'

loop true; do
    input command
    if command == ":help" then
        echo '输入"退款" 开始退款流程\n \
        输入"询问" 开始询问流程\n \
        输入"退出" 可退出服务'
    elif command == "退款"
        call refund
    elif command == "询问"
        call question
    else
        echo "您输入的命令有误，请输入:help查看帮助"
    fi
done
```

## 顶层设计

本项目设计的模块如下

![image-20211128231550028](report.assets/image-20211128231550028.png)

### Parser 

Parser的功能为解释设计的DSL语言，生成可执行代码，符号表。



### VirtualMachine

​	虚拟机的功能类似于 `JVM` 虚拟机，运行`Paser` 解释的结果，并保存`Paser`中的符号表，同时具有`pc`寄存器，用于保存目前运行的结果，并且具有多用户支持的功能



### Web 

​	实现了请求与响应`Mirai`服务器的功能，



### Log

​	日志功能，该模块主要在 `CLI` 环境下交互，以及产生日志文件

​	日志信息包括：`Paser` 的编译错误信息， `VirtualMachine` 的日志信息



### 外部Web框架

​	该模块使用了 `Mirai` 框架提供的 Web 服务器， 该 Web 服务器实现了使用设备配置文件登陆 QQ ，在 OICQ 协议下工作，可以向监听服务器的地址发送 `Json` 消息



## 详细设计

#### Parser
