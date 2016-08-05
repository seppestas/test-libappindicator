all: test-signal
	
test-signal: test-signal.c
	gcc test-signal.c -o test-signal `pkg-config --cflags --libs gtk+-2.0 appindicator-0.1 dbusmenu-gtk-0.4`
