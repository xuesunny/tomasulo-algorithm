#include<bits/stdc++.h>
#include <fstream>
#include"Reorder_buffer.h"
#include"Reservation_station.h"
#include"Schedule.h"
using namespace std;

Reorder_buffer rob;  // reorder_buffer��
Reservation_station rs;  // reservation_station��
Schedule schedule;  // ʱ����������ָ��ִ���������������״̬�����������
int instruction_len;  // ָ������
bool busy[11];  // busy״̬��
int reorder[11];// reorder״̬��
void print(ofstream& outputfile, int num,int n,int m);  // �������������numΪָ��������nΪ��ʼ���ڣ�mΪ��������

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
	int n = 1,m = 1;  // nΪ���״̬����ʼ���ڣ�mΪ��ǰ����
	int entry = 1;
	for (int i = 0; i < 11; i++) {
		busy[i] = 0;
		reorder[i] = -1;
	}
	string s;
	queue<Instruction> q;
	while (getline(inputfile, s)) {  // ��ȡָ����������
		Instruction instruction(s);
		q.push(instruction);
		instruction_len++;
		schedule.inst[instruction_len] = s;
	}
	
	/*ָ�븳ֵ*/
	rob.reorder = reorder;
	rob.busy = busy;
	rob.schedule = &schedule;
	rs.reorder = reorder;
	rs.busy = busy;
	rs.schedule = &schedule;
	rs.rob = &rob;
	schedule.num = instruction_len;  // ��¼ָ����Ŀ

	/*�ȷ�һ��ָ���ʼ��*/
	if (!q.empty()) {
		if (rs.issue(q.front(),entry)) {  // ���Է���
			rob.issue(q.front()); 
			q.pop();
			schedule.issue(entry);
			entry++;
		}
	}

	/*ÿ��ѭ��ִ��һ������*/
	while (1) {
		bool update = false;
		m++;  // ����ʱ��
		schedule.clock = m;
		if (!q.empty()) {  // �����Ƿ���ָ����Է���
			/*�ж��Ƿ��нṹð��*/
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
			if (update == true) {  // ��ָ��ͣ�״̬��Ҫ����
				print(outputfile1, instruction_len, n, m - 1);
				n = m;
			}
			if (update) {  // ���Է���
				rs.issue(q.front(), entry);
				rob.issue(q.front());
				q.pop();
				schedule.issue(entry);
				entry++;
				update = true;
			}
		}
		bool temp = update;  // ��¼��ʱ��update״̬����ǰһ�����Ƿ��Ѿ�д�أ�
		update = (rob.check_update() || rs.check_update());  // �鿴��ִ�������Ƿ���״̬�ı�
		if (temp == false && update == true) {
			print(outputfile1, instruction_len, n, m - 1);
			n = m;
		}
		/*����һ��ʱ������*/
		rob.cycle();
		rs.cycle();
		if (rob.commited > instruction_len) {  // ��ֹ����,����ָ��ύ��
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

