###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-11-29 21:52:03
 # @LastEditors: CYKS
 # @LastEditTime: 2021-11-30 11:35:58
###

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