###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-12-20 19:40:22
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-22 16:56:21
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
		other wrong
	endloop
	echo "再见"

proc wrong:
	echo "输入了未知的功能，请重新输入"
	
proc refund:
  echo "退款"
	
proc order:
  echo "工单"