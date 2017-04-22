
#include<meshy.h>

int main(){
	meshy::EventQueue mainEventQueue(5);
	meshy::IoLoop::get().start();
	return 0;
}

