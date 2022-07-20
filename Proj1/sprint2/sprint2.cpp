#include "sprint2.h"
#include "Testing.h"
#include "ReadFromConsole.h"
#include "FrameworkForTesting.h"


namespace ssprint2 {
	void Sprint2()
	{
		setlocale(LC_ALL, "Russian");
		Testing();
		FrameworkForTesting();
	}
}
