#include "TXCairo.h"
#include "TXLin.h"
#include <cstdio>

int main() {
	txCreateWindow(800, 600);
	txSetFillColor(TX_BLACK);
	txClear();
	printf("%d\n", txBlitPNG("chi.png"));
	txSticky();
	return 0;
}
