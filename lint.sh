###
 # @Descripttion: 
 # @version: 1.0.0
 # @Author: CYKS
 # @Date: 2021-11-30 01:10:09
 # @LastEditors: CYKS
 # @LastEditTime: 2021-12-19 23:58:44
### 
#!/bin/bash

if [ -d "./lint_logd" ]; then
	rm lint_logd/*
else 
	mkdir lint_logd
fi

for dir in parser log test web; do
	for file in ./${dir}/*.cpp ./${dir}/*.hpp; do
		cpplint ${file} >> ./lint_logd/${dir}.log 2>&1
	done	
done