#include"Schedule.h"

Schedule::Schedule()
{
	num = 1;
	clock = 1;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 4; j++) {
			time_table[i][j] = 0;
		}
		inst[i] = "";
	}
}

void Schedule::issue(int entry)
{
	time_table[entry][0] = clock;
}

void Schedule::write_result(int entry)
{
	time_table[entry][1] = clock - 1;
	time_table[entry][2] = clock;
}

void Schedule::commit(int entry)
{
	time_table[entry][3] = clock;
}

void Schedule::print(ofstream& outputfile)
{
	for (int i = 1; i <= num; i++) {
		outputfile << inst[i] << " : ";
		outputfile << time_table[i][0] << " " << time_table[i][1] << " " << time_table[i][2] << " " << time_table[i][3] << endl;
	}
}
