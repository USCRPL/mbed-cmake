#include "mbed.h"

Serial pc(USBTX, USBRX);

int main() {
    pc.baud(115200);

	while(true)
	{
		pc.printf("Hello world!\n");
		wait(1);
	}
}