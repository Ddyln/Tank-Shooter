#include "lib/data.h"
#include "lib/view.h"
#include "lib/control.h"
#include "lib/model.h"

using namespace std;

board a;
gameSound sound;

int main() {

	srand(time(NULL));
	FontSize(21);
	SetConsoleBlank();
	FixConsoleWindow();
	SetConsoleSize(1200, 660);
	HideCursor(1);
	BackGround(0, 1, BLUE);
	Loading();
	while (true) {
		MainMenu(a, sound);
	}
	return 0;
}
