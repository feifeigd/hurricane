
#include<meshy.h>

int main(){
	meshy::EventQueue mainEventQueue(5);
	IoLoop::get().start();
	return 0;
}

