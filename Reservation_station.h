#pragma once
#include"RS_data_load.h"
#include"RS_data_FP.h"
#include "Reorder_buffer.h"
#include "Schedule.h"

class Reservation_station {
public:
	Reorder_buffer* rob;  // 指向reorder buffer的指针，用于协作更新状态
	Schedule* schedule;  // 用于管理schedule表

	int register_status[11];  //记录寄存器的状态（空闲/被用/被写回）
	string value[11];  // 记录计算的结果(储存在reorder buffer里面的值)
	int* reorder;  // 指向算法的reorder数组 
	bool* busy;  // 指向算法的busy数组

	RS_data_load load[2];
	RS_data_FP add[3];
	RS_data_FP mult[2];
	int time_load[2];
	int time_add[3];
	int time_mult[2];

	string address[2];  //暂时记录下Load和store的地址

	/*记录剩余的空位数*/
	int load_left;
	int add_left;
	int mult_left;

	/*记录正在执行哪个单元的指令*/
	int execute_load;
	int execute_add;
	int execute_mult;

	bool load_write;
	bool add_write;
	bool mult_write;

	/*暂存需要写回的数据在周期结束后写回避免冲突*/
	int load_entry, add_entry, mult_entry;
	string load_res, add_res, mult_res;
	bool load_SD, add_SD, mult_SD;

	Reservation_station();
	bool check_update();  // 检查是否需要更新状态
	/*检查各个元件是否有状态需要更新*/
	bool check_update_load();
	bool check_update_add();
	bool check_update_mult();
	bool issue(Instruction instruction,int entry);  // 发送指令
	bool cycle();  // 时钟周期，更新
	// 分别实现时钟更新，返回的布尔值用于观察是否有状态更新
	bool cycle_load();
	bool cycle_add();
	bool cycle_mult();
	void write_back();  // 在周期末尾专门写一个函数写回避免冲突
	void write_result(int entry, string res,bool SD);  // SD用于区分是否为SD指令

	void print(ofstream& outputfile);
};
