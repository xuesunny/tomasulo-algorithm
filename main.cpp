#include<bits/stdc++.h>
#include <fstream>
#include"Reorder_buffer.h"
#include"Reservation_station.h"
#include"Schedule.h"
using namespace std;

Reorder_buffer rob;  // reorder_buffer类
Reservation_station rs;  // reservation_station类
Schedule schedule;  // 时间表，输出最终指令执行情况（所有周期状态输出完后输出）
int instruction_len;  // 指令总数
bool busy[11];  // busy状态表
int reorder[11];// reorder状态表
void print(ofstream& outputfile, int num,int n,int m);  // 定义输出函数，num为指令条数，n为起始周期，m为结束周期

void print(ofstream& outputfile, int num, int n, int m)
{
	outputfile << "cycle_";
	if (n == m) {
		outputfile << n << endl;
	}
	else {
		outputfile << n << "-" << m << endl;
	}
	rob.print(outputfile, num);
	rs.print(outputfile);
	outputfile << "Reorder:";
	for (int i = 0; i < 11; i++) {
		outputfile << "f" << i << ":";
		if (reorder[i] != -1) outputfile << " "<<reorder[i];
		outputfile << ";";
	}
	outputfile << endl;
	outputfile << "Busy:";
	for (int i = 0; i < 11; i++) {
		outputfile << "f" << i << ":";
		if (busy[i]) {
			outputfile << " Yes;";
		}
		else {
			outputfile << "No;";
		}
	}
	outputfile << endl;
}

int main() {
	ifstream inputfile("input1.txt");
	ofstream outputfile1("output11.txt");
	// ofstream outputfile1 << endl;("output11.txt");
	instruction_len = 0;
	int n = 1,m = 1;  // n为这个状态的起始周期，m为当前周期
	int entry = 1;
	for (int i = 0; i < 11; i++) {
		busy[i] = 0;
		reorder[i] = -1;
	}
	string s;
	queue<Instruction> q;
	while (getline(inputfile, s)) {  // 读取指令，进入队列中
		Instruction instruction(s);
		q.push(instruction);
		instruction_len++;
		schedule.inst[instruction_len] = s;
	}
	
	/*指针赋值*/
	rob.reorder = reorder;
	rob.busy = busy;
	rob.schedule = &schedule;
	rs.reorder = reorder;
	rs.busy = busy;
	rs.schedule = &schedule;
	rs.rob = &rob;
	schedule.num = instruction_len;  // 记录指令数目

	/*先发一条指令，初始化*/
	if (!q.empty()) {
		if (rs.issue(q.front(),entry)) {  // 可以发送
			rob.issue(q.front()); 
			q.pop();
			schedule.issue(entry);
			entry++;
		}
	}

	/*每个循环执行一个周期*/
	while (1) {
		bool update = false;
		m++;  // 更新时钟
		schedule.clock = m;
		if (!q.empty()) {  // 看看是否有指令可以发送
			/*判断是否有结构冒险*/
			string op = q.front().op;
			if ((op == "SD" || op == "LD") && rs.load_left > 0) {
				update = true;
			}
			if ((op == "ADDD" || op == "SUBD") && rs.add_left > 0) {
				update = true;
			}
			if ((op == "MULTD" || op == "DIVD") && rs.mult_left > 0) {
				update = true;
			}
			if (update == true) {  // 有指令发送，状态需要更新
				print(outputfile1, instruction_len, n, m - 1);
				n = m;
			}
			if (update) {  // 可以发送
				rs.issue(q.front(), entry);
				rob.issue(q.front());
				q.pop();
				schedule.issue(entry);
				entry++;
				update = true;
			}
		}
		bool temp = update;  // 记录此时的update状态（看前一周期是否已经写回）
		update = (rob.check_update() || rs.check_update());  // 查看在执行周期是否有状态改变
		if (temp == false && update == true) {
			print(outputfile1, instruction_len, n, m - 1);
			n = m;
		}
		/*运行一个时钟周期*/
		rob.cycle();
		rs.cycle();
		if (rob.commited > instruction_len) {  // 终止条件,所有指令都提交了
			break;
		}
	}
	print(outputfile1, instruction_len, n, m );
	outputfile1 << endl;
	outputfile1 << endl;
	outputfile1 << "(Instruction):(Issue cycle),(Exec comp cycle),(Write result cycle), (Commit cycle)" << endl;
	schedule.print(outputfile1);
	inputfile.close();
	outputfile1.close();
	return 0;
}

