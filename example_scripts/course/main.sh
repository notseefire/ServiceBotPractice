###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-12-23 10:34:27
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-23 10:43:26
### 
proc main:
  echo "欢迎"
  echo '可以输入"投诉"或"账单"'
  loop
    loop
      set function
      input function wait 30
      call function = "投诉" complain
      call function = "账单" bill
      call function = "" silence
      other tips
    endloop
    echo "输入1退出"
    set temp
    input temp
    call temp = "1" exit_service
  endloop

  echo "再见"

proc complain:
  echo "输入您的投诉信息"
  set msg
  input msg wait 30
  echo "记录：" + msg

proc bill:
  echo "查看您本月的账单信息"

proc tips:
  echo '可以输入"投诉"或"账单"'

proc silence:
  echo "未接收到消息"

proc exit_service:
  echo "退出"