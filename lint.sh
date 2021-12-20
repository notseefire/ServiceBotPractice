###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-11-30 01:10:09
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-20 09:58:57
### 
#!/bin/bash

if [ -d "./lint_logd" ]; then
	rm lint_logd/*
else 
	mkdir lint_logd
fi

for dir in parser test web; do
	for file in ./${dir}/*.cpp ./${dir}/*.hpp; do
		cpplint --filter=-legal/copyright,-runtime/references,-build/namespaces,-build/include_order ${file} >> ./lint_logd/${dir}.log 2>&1
	done	
done