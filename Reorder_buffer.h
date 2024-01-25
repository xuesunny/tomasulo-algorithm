#pragma once
#include"ROB_data.h"
#include"Schedule.h"

class Reorder_buffer {
public:
	int* reorder;  // 指向算法的reorder数组 
	bool* busy;  // 指向算法的busy数组
	Schedule* schedule;  // 用于管理schedule表

	ROB_data table[15];
	int commited;  // 指向已提交结果的下一项
	int back;  // 指向reorder buffer的最后一个元组

	Reorder_buffer();
	bool check_update();  // 检查是否需要更新表
	bool cycle();  // 每个周期检查一下是否有结果需要提交,返回的布尔值用于观察是否有状态更新
	void issue(Instruction instruction);  // 发送指令

	void print(ofstream& outputfile,int num);  // 输出表的信息
};