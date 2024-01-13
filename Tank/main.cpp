#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"
using namespace std;

board a;

int main() {

	srand(time(NULL));
	FontSize(21);
	SetConsoleBlank();
	FixConsoleWindow();
	SetConsoleSize(1200, 660);
	BackGround(0, 1, BLUE);
	Loading();
	HideCursor(1);
	while (true) {
		MainMenu(a);
	}
	return 0;
}
