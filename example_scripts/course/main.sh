###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-12-23 10:34:27
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-23 12:50:19
### 
proc main:
  echo "欢迎"
  set function
  loop
    loop
      input function wait 30
      call function = "投诉" complain
      call function = "账单" bill
      call function = "注册" register
      call function = "" silence
      call function = "退出" exit_service
      other tips
    endloop
    call function = "退出" null
  endloop

  echo "再见"

proc complain:
  echo "输入您的投诉信息"
  set msg
  input msg wait 30
  echo "记录：" + msg
  assign msg = "记录：" + msg
  store msg "投诉记录"

proc bill:
  echo "查看您本月的账单信息"
  set info
  load info "账单"
  assign info = "余额: " + info
  echo info

proc register:
  set info = "100"
  store info "账单"

proc tips:
  echo '可以输入"投诉"|"账单"|"退出"|"注册”'

proc silence:
  echo "未接收到消息"

proc exit_service:
  echo "退出"