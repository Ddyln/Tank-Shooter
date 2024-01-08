#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"
using namespace std;

board a;

int main() {
	srand(time(NULL));
	SetConsoleBlank();
	FixConsoleWindow();
	SetConsoleSize(1200, 660);
	HideCursor(1);
	while (true) {
		StartGame(a);
	}
	return 0;
}
