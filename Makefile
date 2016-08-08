all: test-unity-panel-service test-menu-about-to-show-signal test-gtk-status-icon

test-unity-panel-service: test-unity-panel-service.c
	gcc test-unity-panel-service.c -o test-unity-panel-service `pkg-config --cflags --libs gtk+-2.0 appindicator-0.1`

test-menu-about-to-show-signal: test-menu-about-to-show-signal.c
	gcc test-menu-about-to-show-signal.c -o test-menu-about-to-show-signal `pkg-config --cflags --libs appindicator-0.1 dbusmenu-gtk-0.4`

test-gtk-status-icon: test-gtk-status-icon.c
	gcc test-gtk-status-icon.c -o test-gtk-status-icon `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm test-unity-panel-service
	rm test-menu-about-to-show-signal
	rm test-gtk-status-icon