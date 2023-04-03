runme: main.c xdg-shell.h xdg-shell.c draw_buf.h draw_buf.c launch-setting.h launch-setting.c
	clang -g main.c launch-setting.c xdg-shell.c draw_buf.c -l wayland-client -l wayland-cursor -o runme

xdg-shell.c:
	wayland-scanner public-code /usr/share/wayland-protocols/unstable/xdg-shell/xdg-shell-unstable-v6.xml xdg-shell.c

xdg-shell.h:
	wayland-scanner client-header /usr/share/wayland-protocols/unstable/xdg-shell/xdg-shell-unstable-v6.xml xdg-shell.h

launch-setting.c:
	wayland-scanner public-code protocol/obric-launch-setting.xml launch-setting.c
launch-setting.h:
	wayland-scanner client-header protocol/obric-launch-setting.xml launch-setting.h