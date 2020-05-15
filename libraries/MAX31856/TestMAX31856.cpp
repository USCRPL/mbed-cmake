#include <mbed.h>

#include <MAX31856.h>

int main()
{
	Serial pc(USBTX, USBRX);
	pc.baud(115200);

	// TODO: create and init MAX31856 driver

	while(true)
	{
		pc.printf("\nMAX31856 Test Suite:\n");


		int test = -1;

		//Menu
		pc.printf("Select a test: \n");
		pc.printf("1.  Check Existence\n");
		pc.printf("9.  Exit Test Suite\n");

		pc.scanf("%d", &test);
		pc.printf("Running test %d:\n\n", test);


		//Run Tests
		switch(test)
		{
			case 1 :
				// TODO: check existence
				break;
			case 9 :
				pc.printf("Exiting Test Suite\n");
				return 0;
			default :
				pc.printf("Invalid Test Number Selection.\n");
		}

		pc.printf("Done.\r\n");

	}
}