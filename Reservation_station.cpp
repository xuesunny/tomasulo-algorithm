#include"Reservation_station.h"

 static int get_number(string s) { // 读取字符串中的数字
	string num = "";
	int i = 0;
	while (i < s.length() && !isdigit(s[i])) i++;
	while (i < s.length() && isdigit(s[i])) {
		num += s[i];
		i++;
	}
	return stoi(num);  // 转为整数
}

Reservation_station::Reservation_station() {
	rob = NULL;
	schedule = NULL;
	load_left = 2;
	add_left = 3;
	mult_left = 2;
	time_load[0] = time_load[1] = -1;
	time_add[0] = time_add[1] = time_add[2] = -1;
	time_mult[0] = time_mult[1] = -1;
	load[0].name = "Load1";
	load[1].name = "Load2";
	add[0].name = "Add1";
	add[1].name = "Add2";
	add[2].name = "Add3";
	mult[0].name = "Mult1";
	mult[1].name = "Mult2";
	for (int i = 0; i < 11; i++) {
		register_status[i] = 0;
		value[i] = "";
	}
	execute_add = execute_load = execute_mult = 0;
	load_write = false;
	add_write = false;
	mult_write = false;
	load_entry = add_entry = mult_entry = -1;
	load_res = add_res = mult_res = "";
	load_SD = add_SD = mult_SD = false;
}

bool Reservation_station::check_update()  // 检查是否有issue转execute，execute转write_back，A地址改变情况
{
	bool update1, update2, update3;
	update1 = check_update_load();
	update2 = check_update_add();
	update3 = check_update_mult();
	return (update1 || update2 || update3);
}

bool Reservation_station::check_update_load()
{
	int i = 0;
	i = execute_load;
	if (load[i].busy) {
		if (load[i].Qj == 0 && load[i].Qk == 0) {
			if (time_load[i] == 2 || time_load[i] == 0) {  // 进入execute（同时改变A地址），或进入write result
				return true;
			}
		}
	}
	return false;
}

bool Reservation_station::check_update_add()
{
	int i = 0;
	i = execute_add;
	if (add[i].busy) {
		if (add[i].Qj == 0 && add[i].Qk == 0) {
			if (time_add[i] == 0 || time_add[i]==2) {   // 进入execute或进入write result
				return true;
			}
		}
	}
	return false;
}

bool Reservation_station::check_update_mult()
{
	int i = 0;
	i = execute_mult;
	if (mult[i].busy) {
		if (mult[i].Qj == 0 && mult[i].Qk == 0) {
			if (time_mult[i] == 0) {  // 进入write result
				return true;
			}
			if (time_mult[i] == 10 && mult[i].op == "MULTD") {  // 进入excute
				return true;
			}
			if (time_mult[i] == 20 && mult[i].op == "DVID") {  // 进入excute
				return true;
			}
		}
	}
	return false;
}

bool Reservation_station::issue(Instruction instruction,int entry) {
	/*ISSUE 指令LD和SD*/
	if (instruction.op == "LD" || instruction.op == "SD") {
		if (load_left == 0) {  // 结构冒险
			return false;
		}
		else {
			load_left--;
			int i = 0;
			while (i<2 && load[i].busy) i++; // 找到空位位置
			if (i == 2) cout << "load单元出问题了" << endl;
			time_load[i] = 2;
			int rs = get_number(instruction.dest);
			load[i].busy = 1;
			load[i].op = instruction.op;
			load[i].Dest = entry;
			load[i].A = instruction.sources1;  // 此时A为立即数,并不是最终地址
			string addr="";
			addr += to_string(get_number(instruction.sources1));
			addr += " + Reg[";
			addr += instruction.sources2+"]";
			address[i] = addr;  // 暂存地址，下个周期写入A中
			if (instruction.op == "LD") {  //LD指令
				busy[rs] = 1;
				reorder[rs] = entry;
				register_status[rs] = 1;
			}
			else {  // SD指令
				if (register_status[rs] != 0 && reorder[rs]!=-1) {  // 需要等待操作数读完
					load[i].Qj = reorder[rs];
				}
			}
		}
	}

	/*ISSUE 指令ADDD和SUBD*/
	if (instruction.op == "ADDD" || instruction.op == "SUBD") {
		if (add_left == 0) {
			return false;  // 结构冒险
		}
		else {
			add_left--;
			int i = 0;
			while (i<3 && add[i].busy) i++; // 找到空位位置
			if (i == 3) cout << "add单元出问题了" << endl;
			time_add[i] = 2;
			add[i].busy = 1;
			add[i].op = instruction.op;
			add[i].Dest= entry;
			int rt = get_number(instruction.sources1);
			int rd = get_number(instruction.sources2);
			if (register_status[rt]!=0 && reorder[rt] != -1) {  // 需要等待算完
				add[i].Qj = reorder[rt];
			}
			else if (register_status[rt] == 0 && reorder[rt] != -1) {  // 已经算完，存在reorder buffer的value中
				add[i].vj = value[rt];
				add[i].Qj = 0;
			}
			else {  // 直接读取寄存器的值
				add[i].vj = "Regs[" + instruction.sources1 + "]";
				add[i].Qj = 0;
			}

			if (register_status[rd] != 0 &&  reorder[rd] != -1) {  // 需要等待算完
				add[i].Qk = reorder[rd];
			}
			else if (register_status[rd] == 0 && reorder[rd] != -1) {  // 已经算完，存在reorder buffer的value中
				add[i].vk = value[rd];
				add[i].Qk = 0;
			}
			else {  // 直接读取寄存器的值
				add[i].vk = "Regs[" + instruction.sources2 + "]";
				add[i].Qk = 0;
			}
			int rs = get_number(instruction.dest);
			busy[rs] = 1;
			reorder[rs] = entry;
			register_status[rs] = 1;
		}
	}

	/*ISSUE 指令MULTD和DIVID*/
	if (instruction.op == "MULTD" || instruction.op == "DIVD") {
		if (mult_left == 0) {  
			return false;  // 结构冒险
		}
		else {
			mult_left--;
			int i = 0;
			while (i<2 && mult[i].busy) i++; // 找到空位位置
			if (i == 2) cout << "mult单元出问题了" << endl;
			if (instruction.op == "MULTD") {
				time_mult[i] = 10;
			}
			if (instruction.op == "DIVD") {
				time_mult[i] = 20;
			}
			mult[i].busy = 1;
			mult[i].op = instruction.op;
			mult[i].Dest = entry;
			int rt = get_number(instruction.sources1);
			int rd = get_number(instruction.sources2);
			if (register_status[rt] != 0 && reorder[rt] != -1) {  // 需要等待算完
				mult[i].Qj = reorder[rt];
			}
			else if (register_status[rt] == 0 && reorder[rt] != -1) {  // 已经算完，存在reorder buffer的value中
				mult[i].vj = value[rt];
				mult[i].Qj = 0;
			}
			else {  // 直接读取寄存器的值
				mult[i].vj = "Regs[" + instruction.sources1 + "]";
				mult[i].Qj = 0;
			}

			if (register_status[rd] != 0 && reorder[rd] != -1) {  // 需要等待算完
				mult[i].Qk = reorder[rd];
			}
			else if (register_status[rd] == 0 && reorder[rd] != -1) {  // 已经算完，存在reorder buffer的value中
				mult[i].vk = value[rd];
				mult[i].Qk = 0;
			}
			else {  // 直接读取寄存器的值
				mult[i].vk = "Regs[" + instruction.sources2 + "]";
				mult[i].Qk = 0;
			}
			int rs = get_number(instruction.dest);
			busy[rs] = 1;
			reorder[rs] = entry;
			register_status[rs] = 1;
		}
	}
	return true;
}

bool Reservation_station::cycle() {  // 模拟一个时钟周期执行的东西
	bool update1, update2, update3;
	update1=cycle_load();
	update2=cycle_add();
	update3=cycle_mult();
	write_back();

	return (update1 || update2 || update3);
}

bool Reservation_station::cycle_load() {
	load_write = false;  // 初始化没有写回
	bool update = false;  // 记录是否有状态更新
	bool write_back = false;  // 记录是否需要写回
	string s = "";  // 用于记录计算结果
	int WR_entry = 0;  // 记录写回的目的地
	int i = 0;
	bool SD = false;  // 判断是否为SD指令
	/*更新load的状态*/
	i = execute_load;
	if (load[i].busy == 1) {
		if (load[i].Qj == 0 && load[i].Qk == 0 && time_load[i] > 0) {
			if (time_load[i] == 2) {  // ROB状态转为Excution
				rob->table[load[i].Dest].state = 2;
				update = true;
			}
			time_load[i]--;
			if (time_load[i] == 1) {  // 计算出地址
				load[i].A = address[i];
				update = true;
			}
		}
		else if (load[i].Qj == 0 && load[i].Qk == 0 && time_load[i] == 0) {  // 进入write result阶段
			if (load[i].op == "LD") {
				s += "Mem[";
				s += load[i].A + "]";
				write_back = true;
				WR_entry = load[i].Dest;
			}
			else {  // 指令SD
				write_back = true;
				WR_entry = load[i].Dest;
				SD = true;
			}
			/*清空这一栏*/
			load[i].busy = 0; load[i].op = "";
			load[i].vj = load[i].vk = "";
			load[i].Qj = load[i].Qk = 0;
			load[i].Dest = -1;
			load[i].A = "";
			load_left++;  // 释放资源
		}
	}
	if (load[i].busy == 0) {  // 找下一个执行目标
		if (load[1 - execute_load].busy == 1) {
			execute_load = 1 - execute_load;
			cycle_load();
		}
		else execute_load = 0;
	}
	if (write_back) {
		load_write = true;
		load_entry = WR_entry;
		load_res = s;
		load_SD = SD;
		update = true;
	}
	return update;
}

bool Reservation_station::cycle_add() {
	add_write = false;
	bool update=false;  // 记录是否有状态更新
	bool write_back = false;  // 记录是否需要写回
	string s = "";  // 用于记录计算结果
	int WR_entry = 0;  // 记录写回的目的地
	int i = 0;
	bool SD = false; // 判断是否为SD指令
	/*更新add的状态*/
	i = execute_add;
	if (add[i].busy == 1) {
		if (add[i].Qj == 0 && add[i].Qk == 0 && time_add[i] > 0) {
			if (time_add[i] == 2) {  // ROB状态转为Excution
				rob->table[add[i].Dest].state = 2;
				update = true;
			}
			time_add[i]--;
		}
		else if (add[i].Qj == 0 && add[i].Qk == 0 && time_add[i] == 0) {  // 进入write result阶段
			if (add[i].op == "ADDD") {
				s +="(" + add[i].vj + " + " + add[i].vk + ")";
			}
			if (add[i].op == "SUBD") {
				s+= "("+ add[i].vj + " - " + add[i].vk + ") ";
			}
			write_back = true;
			WR_entry = add[i].Dest;
			/*清空这一栏*/
			add[i].busy = 0; add[i].op = ""; 
			add[i].vj = add[i].vk = "";
			add[i].Qj = add[i].Qk = 0;
			add[i].Dest = -1;
			add[i].A = "";
			add_left++;  // 释放资源
		}
	}
	if (add[i].busy == 0) {  // 找下一个执行目标
		int min_entry = 100;
		int min_i = 0;
		for (i = 0; i < 3; i++) {
			if (add[i].busy && add[i].Dest < min_entry) {
				min_entry = add[i].Dest;
				min_i = i;
			}
		}
		if (min_entry != 100) {
			execute_add = min_i;
			cycle_add();
		}
		else {
			execute_add = 0;
		}
	}
	if (write_back) {
		add_write = true;
		add_entry = WR_entry;
		add_res = s;
		add_SD = SD;
		update = 1;
	}
	return update;
}

bool Reservation_station::cycle_mult() {
	mult_write = false;
	bool update = false;  // 记录是否有状态更新
	bool write_back = false;  // 记录是否需要写回
	int i = 0;
	string s = "";  // 用于记录计算结果
	int WR_entry = 0;  // 记录写回的目的地
	bool SD = false;  // 判断是否为SD指令
	/*更新mult的状态*/
	i = execute_mult;
	if (mult[i].busy == 1) {
		if (mult[i].Qj == 0 && mult[i].Qk == 0 && time_mult[i] > 0) {
			if (time_mult[i] == 10 && mult[i].op=="MULTD") {  // ROB状态转为Excution
				rob->table[mult[i].Dest].state = 2;
				update = true;
			}
			if (time_mult[i] == 20 && mult[i].op == "DIVD") {  // ROB状态转为Excution
				rob->table[mult[i].Dest].state = 2;
				update = true;
			}
			time_mult[i]--;
		}
		else if (mult[i].Qj == 0 && mult[i].Qk == 0 && time_mult[i] == 0) {  // 进入write result阶段
			if (mult[i].op == "MULTD") {
				s += mult[i].vj + " * " + mult[i].vk;
			}
			if (mult[i].op == "DIVD") {
				s += mult[i].vj + " / " + mult[i].vk;
			}
			write_back = true;
			WR_entry = mult[i].Dest;
			/*清空这一栏*/
			mult[i].busy = 0; mult[i].op = "";
			mult[i].vj = mult[i].vk = "";
			mult[i].Qj = mult[i].Qk = 0;
			mult[i].Dest = -1;
			mult[i].A = "";
			mult_left++;  // 释放资源
		}
	}
	if (mult[i].busy == 0) {  // 找下一个执行目标
		if (mult[1 - execute_mult].busy == 1) {
			execute_mult = 1 - execute_mult;
			cycle_mult();
		}
		else execute_mult = 0;
	}
	if (write_back) {
		mult_write = true;
		mult_entry = WR_entry;
		mult_res = s;
		mult_SD = SD;
		update = true;
	}
	return update;
}

void Reservation_station::write_back()  // 轮询，看是否有需要更新的
{
	if (load_write) {
		write_result(load_entry, load_res, load_SD);
		return;
	}
	if (add_write) {
		write_result(add_entry, add_res, add_SD);
		return;
	}
	if (mult_write) {
		write_result(mult_entry, mult_res, mult_SD);
		return;
	}
}

void Reservation_station::write_result(int entry, string res,bool SD) {
	schedule->write_result(entry);
	if (SD==false) {
		/*CDB广播*/
		for (int i = 0; i < 2; i++) {
			if (load[i].Qj == entry) {
				load[i].vj = res;
				load[i].Qj = 0;
			}
			if (load[i].Qk == entry) {
				load[i].vk = res;
				load[i].Qk = 0;
			}
		}
		for (int i = 0; i < 3; i++) {
			if (add[i].Qj == entry) {
				add[i].vj = res;
				add[i].Qj = 0;
			}
			if (add[i].Qk == entry) {
				add[i].vk = res;
				add[i].Qk = 0;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (mult[i].Qj == entry) {
				mult[i].vj = res;
				mult[i].Qj = 0;
			}
			if (mult[i].Qk == entry) {
				mult[i].vk = res;
				mult[i].Qk = 0;
			}
		}
		// 更新ROB
		rob->table[entry].state = 3;
		rob->table[entry].value = res;
		// 更新value和register_status
		int index = rob->table[entry].dest_reg;  // 寄存器目的地址
		register_status[index] = 0;
		value[index] = res;
	}
	else {
		// 更新ROB
		rob->table[entry].state = 3;
		rob->table[entry].value = res;
	}
}

void Reservation_station::print(ofstream& outputfile) {
	for (int i = 0; i < 2; i++) {
		load[i].print(outputfile);
	}
	for (int i = 0; i < 3; i++) {
		add[i].print(outputfile);
	}
	for (int i = 0; i < 2; i++) {
		mult[i].print(outputfile);
	}
}