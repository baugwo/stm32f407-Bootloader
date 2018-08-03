#include "stm32f4xx.h"
#include "IAP_FUN.h"

int main()
{
    SystemInit();
		IAP_fun_run();
		while(1)
		{}
}
