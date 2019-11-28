#include <X11/Xlib.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define OPERATION_MV (uint)1
#define OPERATION_C (uint)2

void mouseClick(int button)
{
	Display *display = XOpenDisplay(NULL);

	XEvent event;

	if (display == NULL)
	{
		fprintf(stderr, "Errore nell'apertura del Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonPress;
	event.xbutton.button = button;
	event.xbutton.same_screen = True;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window,
				  &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
				  &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while (event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow,
					  &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
					  &event.xbutton.state);
	}

	if (XSendEvent(display, event.xbutton.window, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

	XFlush(display);

	usleep(100000);

	event.type = ButtonRelease;
	event.xbutton.state = 0x100;

	if (XSendEvent(display, event.xbutton.window, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

	XFlush(display);

	XCloseDisplay(display);
}

void mouseMove(int x, int y)
{
	Display *display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);
	XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
	XFlush(display);
	usleep(1000);
	XCloseDisplay(display);
}


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: <operation> <?x> <?y> <?button>\n");
		return EXIT_FAILURE;
	}

	unsigned int op = 0;

	if (strcmp("move", argv[1]) == 0 || strcmp("mv", argv[1]) == 0)
	{
		op = OPERATION_MV;
	} else if (strcmp("click", argv[1]) == 0 || strcmp("c", argv[1]) == 0)
	{
		op = OPERATION_C;
	} else if (strcmp("move-click", argv[1]) == 0 || strcmp("mc", argv[1]) == 0)
	{
		op = OPERATION_MV | OPERATION_C;
	}

	if (!op)
	{
		fprintf(stderr, "unsupported operation\n");
		return EXIT_FAILURE;
	}


	if (op & OPERATION_C && !(op & OPERATION_MV))
	{
		if (argc < 3)
		{
			fprintf(stderr, "Usage: XMouse click <button>\n");
			return EXIT_FAILURE;
		}
		int button = (int) strtol(argv[2], NULL, 10);
		mouseClick(button);
	} else {
		if (argc < 4)
		{
			fprintf(stderr, "Usage: <operation> <?x> <?y> <?button>\n");
			return EXIT_FAILURE;
		}
		int mouseX = (int) strtol(argv[2], NULL, 10);
		int mouseY = (int) strtol(argv[3], NULL, 10);

		if (op & OPERATION_MV)
		{
			mouseMove(mouseX, mouseY);
		}

		if (op & OPERATION_C)
		{
			if (argc < 5)
			{
				fprintf(stderr, "please provide button type [1, 2, 3]\n");
				return EXIT_FAILURE;
			}
			int button = (int) strtol(argv[4], NULL, 10);
			mouseClick(button);
			sleep(1);
			mouseClick(button);
		}
	}
	return EXIT_SUCCESS;
}