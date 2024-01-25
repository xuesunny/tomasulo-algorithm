#pragma once
#include"Instruction.h"

/*���ڼ�¼ issue�� excute�� write result��commit��ʱ��*/
class Schedule {
public:
	int clock;  // ��¼��ǰʱ������
	int num;  // ָ�����Ŀ
	int time_table[15][4];  // �洢ָ��״̬
	string inst[15];

	Schedule();
	void issue(int entry);  // ��ָ����Ҫ����
	void write_result(int entry); // ��ָ��д�أ�execute complete��д�ص�ǰһ�����ڣ�
	void commit(int entry);  // ��ָ���ύ
	void print(ofstream& outputfile);
};