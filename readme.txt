详细内容可见实验报告.pdf

这是一个tomasulo算法的简单实现。可以处理LOAD,STORE,ADDD,SUBD,DIVID,MULTD指令，并跑通Input1.txt和input2.txt两个样例
输出Output11.txt,output21.txt为每个时钟周期状态，并在最后输出最终指令最终执行情况表。

输入为Input1.txt的输出为output11.txt
输入为Input2.txt的输出为output21.txt
通过更改main.cpp里面的文件路径可以决定输入的是input1还是Input2
这些都位于code文件夹，体系结构大作业1文件夹下

遇到visual studio报错及解决：
1、对我的#include<bits/stdc++.h>报错
解决方法参考网址：https://blog.csdn.net/p19983491210/article/details/127001488
2、对旧版本字符串处理报错，如对strtok,strcat这些报错
参考网址：https://blog.csdn.net/qq_45863239/article/details/115434272
