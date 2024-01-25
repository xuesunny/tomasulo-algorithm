#pragma once
#include"RS_data_load.h"
#include"RS_data_FP.h"
#include "Reorder_buffer.h"
#include "Schedule.h"

class Reservation_station {
public:
	Reorder_buffer* rob;  // ָ��reorder buffer��ָ�룬����Э������״̬
	Schedule* schedule;  // ���ڹ���schedule��

	int register_status[11];  //��¼�Ĵ�����״̬������/����/��д�أ�
	string value[11];  // ��¼����Ľ��(������reorder buffer�����ֵ)
	int* reorder;  // ָ���㷨��reorder���� 
	bool* busy;  // ָ���㷨��busy����

	RS_data_load load[2];
	RS_data_FP add[3];
	RS_data_FP mult[2];
	int time_load[2];
	int time_add[3];
	int time_mult[2];

	string address[2];  //��ʱ��¼��Load��store�ĵ�ַ

	/*��¼ʣ��Ŀ�λ��*/
	int load_left;
	int add_left;
	int mult_left;

	/*��¼����ִ���ĸ���Ԫ��ָ��*/
	int execute_load;
	int execute_add;
	int execute_mult;

	bool load_write;
	bool add_write;
	bool mult_write;

	/*�ݴ���Ҫд�ص����������ڽ�����д�ر����ͻ*/
	int load_entry, add_entry, mult_entry;
	string load_res, add_res, mult_res;
	bool load_SD, add_SD, mult_SD;

	Reservation_station();
	bool check_update();  // ����Ƿ���Ҫ����״̬
	/*������Ԫ���Ƿ���״̬��Ҫ����*/
	bool check_update_load();
	bool check_update_add();
	bool check_update_mult();
	bool issue(Instruction instruction,int entry);  // ����ָ��
	bool cycle();  // ʱ�����ڣ�����
	// �ֱ�ʵ��ʱ�Ӹ��£����صĲ���ֵ���ڹ۲��Ƿ���״̬����
	bool cycle_load();
	bool cycle_add();
	bool cycle_mult();
	void write_back();  // ������ĩβר��дһ������д�ر����ͻ
	void write_result(int entry, string res,bool SD);  // SD���������Ƿ�ΪSDָ��

	void print(ofstream& outputfile);
};
