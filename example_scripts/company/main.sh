###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-12-20 19:40:22
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-23 13:01:57
### 
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
	echo "退款" + number + "个" + item 
	set record
	assign record = number + "," + item 
	store record "退款"
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