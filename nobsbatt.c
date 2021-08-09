/* Copyright 2021, Dr Ashton Fagg <ashton@fagg.id.au>

* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all
* copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
* AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
* DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
* PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
* TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
* PERFORMANCE OF THIS SOFTWARE. */

/* nobsbatt - A no bullshit battery monitor for OpenBSD. */

#include "config.h" /* User settings go here */

#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

#include <machine/apmvar.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <err.h>
#include <unistd.h>

static int fd_apm;      /* File descriptor for apm */

/* Function prototypes */
void timer_setup(struct timeval *);
int apm_ioctl(struct apm_power_info *);
char *make_acdc_str(struct apm_power_info *);
char *make_perc_str(struct apm_power_info *);
char *make_time_str(struct apm_power_info *);

/* Helpful macros */

#define APM_ON_BATT(x)\
	((x)->ac_state == APM_AC_OFF || (x)->ac_state == APM_AC_BACKUP)

#define APM_ON_AC(x)\
	((x)->ac_state == APM_AC_ON)

int
main()
{
	struct apm_power_info *apm_status = NULL;
	char *acdc_str, *perc_str, *time_str;

	/* X11 shit */
	int fd_xserv;
	XftDraw *draw;
	XftFont *font;
	XftColor color;
	Display *display;
	int screen;
	Window window;
	GC gc;
	fd_set fdset;
	XRenderColor xrc = TEXT_COLOR;
	XEvent xev;
	struct timeval timer;

	apm_status = (struct apm_power_info *)malloc(sizeof(struct apm_power_info));
	if (apm_status == NULL)
		err(1, "malloc");

	if ((fd_apm = open(APM_DEV, O_RDONLY)) < 0)
		err(1, "Could not open APM device: %s", APM_DEV);

	display = XOpenDisplay((char *)0);
	screen = DefaultScreen(display);
	fd_xserv = ConnectionNumber(display);
	font = XftFontOpenName(display, DefaultScreen(display), FONT_NAME);

	/* xxx need to find a way to handle things like "top left",
	 * "bottom right" etc. but whatever, this'll do for now */
	window = XCreateSimpleWindow(display,
	    DefaultRootWindow(display),
	    0,
	    0,
	    WINDOW_SIZE,
	    WINDOW_SIZE,
	    5,
	    FG_COLOR,
	    BG_COLOR);

	XSetStandardProperties(display, window, WINDOW_NAME, WINDOW_NAME,
	    None, NULL, 0, NULL);

	draw = XftDrawCreate(display, window, DefaultVisual(display, 0),
	    DefaultColormap(display, 0));

	gc = XCreateGC(display, window, 0, 0);
	XSetBackground(display, gc, BG_COLOR);
	XSetForeground(display, gc, FG_COLOR);
	XSetFillStyle(display, gc, FillSolid);
	XClearWindow(display, window);
	XMapRaised(display, window);
	XftColorAllocValue(display, DefaultVisual(display, screen),
	    DefaultColormap(display, screen), &xrc, &color);

	while (1) {
		FD_ZERO(&fdset);
		FD_SET(fd_xserv, &fdset);

		timer_setup(&timer);

		if (!select(fd_xserv + 1, &fdset, 0, 0, &timer)) {
			if (apm_ioctl(apm_status) < 0)
				err(1, "apm ioctl");

			acdc_str = make_acdc_str(apm_status);
			perc_str = make_perc_str(apm_status);

			XClearWindow(display, window);

			/* xxx is there a way to make the positioning
			 * less brittle? */
			XftDrawStringUtf8(draw, &color, font, 5, WINDOW_SIZE/4,
			    (XftChar8 *)perc_str, strlen(perc_str));

			XftDrawStringUtf8(draw, &color, font, 5,
			    3.75 * (WINDOW_SIZE/4),
			    (XftChar8 *)acdc_str, strlen(acdc_str));

			free(acdc_str);
			free(perc_str);

			if (APM_ON_BATT(apm_status)) {
				time_str = make_time_str(apm_status);
				XftDrawStringUtf8(draw, &color, font, 5,
				    (WINDOW_SIZE/2) + 10,
				    (XftChar8 *)time_str, strlen(time_str));
				free(time_str);
			}
		}

		while (XPending(display))
			XNextEvent(display, &xev);
	}

	free(apm_status);
	close(fd_apm);
	XFreeGC(display, gc);
	XDestroyWindow(display, window);
	XCloseDisplay(display);

	return 0;
}

void
timer_setup(struct timeval *timer)
{
	timer->tv_usec = 0;
	timer->tv_sec = POLL_SECONDS;
}

int
apm_ioctl(struct apm_power_info *apm_status)
{
	if (ioctl(fd_apm, APM_IOC_GETPOWER, apm_status) == -1)
		return -1;

	/* Make sure we have something we understand. */
	switch (apm_status->ac_state) {
	case APM_AC_OFF:
	case APM_AC_ON:
	case APM_AC_BACKUP:
	case APM_AC_UNKNOWN:
		return 0;
	default:
		fprintf(stderr, "Got an APM status we don't support\n");
	}

	return -1;
}

char *
make_acdc_str(struct apm_power_info *status)
{
	char *str = calloc(9, sizeof(char));

	if (APM_ON_AC(status))
		asprintf(&str, "Power: AC");
	else if (APM_ON_BATT(status))
		asprintf(&str, "Power: DC");
	else
		asprintf(&str, "Power: ?");

	return str;
}

char *
make_perc_str(struct apm_power_info *status)
{
	char *str = calloc(9, sizeof(char));
	unsigned int perc = (unsigned int)status->battery_life;

	asprintf(&str, "Cap: %3d%%", perc);
	return str;
}

char *
make_time_str(struct apm_power_info *status)
{
	char *str = calloc(9, sizeof(char));
	unsigned int mins = (unsigned int)status->minutes_left;
	unsigned int hrs = 0;

	if (mins >= 60) {
		hrs = mins / 60;
		mins = mins % 60;
	}

	asprintf(&str, "Est:%2d:%02d", hrs, mins);
	return str;
}
