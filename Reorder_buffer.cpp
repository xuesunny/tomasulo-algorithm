#include "Reorder_buffer.h"

Reorder_buffer::Reorder_buffer()
{
	schedule = NULL;
	reorder = NULL;
	busy = NULL;
	commited = 1;
	back = 1;
	for (int i = 1; i < 15; i++) {
		table[i].entry = i;
	}
}

bool Reorder_buffer::check_update()  // 当有指令需要commit的时候，触发更新操作
{	
	if (table[commited].state == 3) {
		return true;
	}
	else {
		return false;
	}
}

void Reorder_buffer::issue(Instruction instruction)
{
	ROB_data rob_data(instruction, back);
	rob_data.busy = 1;
	table[back] = rob_data;
	/*更新busy和reorder数组*/
	busy[rob_data.dest_reg] = 1;
	reorder[rob_data.dest_reg] = back;
	back++;
}

bool Reorder_buffer::cycle()
{
	if (table[commited].state == 3) {  // 提交数据
		table[commited].state = 4;
		table[commited].busy = 0;
		busy[table[commited].dest_reg] = 0;
		reorder[table[commited].dest_reg] = -1;
		schedule->commit(commited);
		commited++;
		return true;
	}
	return false;
}

void Reorder_buffer::print(ofstream& outputfile, int num)
{
	for (int i = 1; i <= num; i++) {
		table[i].print(outputfile);
	}
}

