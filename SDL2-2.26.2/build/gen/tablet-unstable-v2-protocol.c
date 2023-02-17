/* Generated by wayland-scanner 1.18.0 */

/*
 * Copyright 2014 © Stephen "Lyude" Chandler Paul
 * Copyright 2015-2016 © Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with non-clang compilers. */
#endif

#if (__has_attribute(visibility) || defined(__GNUC__) && __GNUC__ >= 4)
#define WL_PRIVATE __attribute__ ((visibility("hidden")))
#else
#define WL_PRIVATE
#endif

extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface zwp_tablet_pad_group_v2_interface;
extern const struct wl_interface zwp_tablet_pad_ring_v2_interface;
extern const struct wl_interface zwp_tablet_pad_strip_v2_interface;
extern const struct wl_interface zwp_tablet_pad_v2_interface;
extern const struct wl_interface zwp_tablet_seat_v2_interface;
extern const struct wl_interface zwp_tablet_tool_v2_interface;
extern const struct wl_interface zwp_tablet_v2_interface;

static const struct wl_interface *tablet_unstable_v2_types[] = {
	NULL,
	NULL,
	NULL,
	&zwp_tablet_seat_v2_interface,
	&wl_seat_interface,
	&zwp_tablet_v2_interface,
	&zwp_tablet_tool_v2_interface,
	&zwp_tablet_pad_v2_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&zwp_tablet_v2_interface,
	&wl_surface_interface,
	&zwp_tablet_pad_ring_v2_interface,
	&zwp_tablet_pad_strip_v2_interface,
	&zwp_tablet_pad_group_v2_interface,
	NULL,
	&zwp_tablet_v2_interface,
	&wl_surface_interface,
	NULL,
	&wl_surface_interface,
};

static const struct wl_message zwp_tablet_manager_v2_requests[] = {
	{ "get_tablet_seat", "no", tablet_unstable_v2_types + 3 },
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_manager_v2_interface = {
	"zwp_tablet_manager_v2", 1,
	2, zwp_tablet_manager_v2_requests,
	0, NULL,
};

static const struct wl_message zwp_tablet_seat_v2_requests[] = {
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_seat_v2_events[] = {
	{ "tablet_added", "n", tablet_unstable_v2_types + 5 },
	{ "tool_added", "n", tablet_unstable_v2_types + 6 },
	{ "pad_added", "n", tablet_unstable_v2_types + 7 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_seat_v2_interface = {
	"zwp_tablet_seat_v2", 1,
	1, zwp_tablet_seat_v2_requests,
	3, zwp_tablet_seat_v2_events,
};

static const struct wl_message zwp_tablet_tool_v2_requests[] = {
	{ "set_cursor", "u?oii", tablet_unstable_v2_types + 8 },
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_tool_v2_events[] = {
	{ "type", "u", tablet_unstable_v2_types + 0 },
	{ "hardware_serial", "uu", tablet_unstable_v2_types + 0 },
	{ "hardware_id_wacom", "uu", tablet_unstable_v2_types + 0 },
	{ "capability", "u", tablet_unstable_v2_types + 0 },
	{ "done", "", tablet_unstable_v2_types + 0 },
	{ "removed", "", tablet_unstable_v2_types + 0 },
	{ "proximity_in", "uoo", tablet_unstable_v2_types + 12 },
	{ "proximity_out", "", tablet_unstable_v2_types + 0 },
	{ "down", "u", tablet_unstable_v2_types + 0 },
	{ "up", "", tablet_unstable_v2_types + 0 },
	{ "motion", "ff", tablet_unstable_v2_types + 0 },
	{ "pressure", "u", tablet_unstable_v2_types + 0 },
	{ "distance", "u", tablet_unstable_v2_types + 0 },
	{ "tilt", "ff", tablet_unstable_v2_types + 0 },
	{ "rotation", "f", tablet_unstable_v2_types + 0 },
	{ "slider", "i", tablet_unstable_v2_types + 0 },
	{ "wheel", "fi", tablet_unstable_v2_types + 0 },
	{ "button", "uuu", tablet_unstable_v2_types + 0 },
	{ "frame", "u", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_tool_v2_interface = {
	"zwp_tablet_tool_v2", 1,
	2, zwp_tablet_tool_v2_requests,
	19, zwp_tablet_tool_v2_events,
};

static const struct wl_message zwp_tablet_v2_requests[] = {
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_v2_events[] = {
	{ "name", "s", tablet_unstable_v2_types + 0 },
	{ "id", "uu", tablet_unstable_v2_types + 0 },
	{ "path", "s", tablet_unstable_v2_types + 0 },
	{ "done", "", tablet_unstable_v2_types + 0 },
	{ "removed", "", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_v2_interface = {
	"zwp_tablet_v2", 1,
	1, zwp_tablet_v2_requests,
	5, zwp_tablet_v2_events,
};

static const struct wl_message zwp_tablet_pad_ring_v2_requests[] = {
	{ "set_feedback", "su", tablet_unstable_v2_types + 0 },
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_pad_ring_v2_events[] = {
	{ "source", "u", tablet_unstable_v2_types + 0 },
	{ "angle", "f", tablet_unstable_v2_types + 0 },
	{ "stop", "", tablet_unstable_v2_types + 0 },
	{ "frame", "u", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_pad_ring_v2_interface = {
	"zwp_tablet_pad_ring_v2", 1,
	2, zwp_tablet_pad_ring_v2_requests,
	4, zwp_tablet_pad_ring_v2_events,
};

static const struct wl_message zwp_tablet_pad_strip_v2_requests[] = {
	{ "set_feedback", "su", tablet_unstable_v2_types + 0 },
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_pad_strip_v2_events[] = {
	{ "source", "u", tablet_unstable_v2_types + 0 },
	{ "position", "u", tablet_unstable_v2_types + 0 },
	{ "stop", "", tablet_unstable_v2_types + 0 },
	{ "frame", "u", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_pad_strip_v2_interface = {
	"zwp_tablet_pad_strip_v2", 1,
	2, zwp_tablet_pad_strip_v2_requests,
	4, zwp_tablet_pad_strip_v2_events,
};

static const struct wl_message zwp_tablet_pad_group_v2_requests[] = {
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_pad_group_v2_events[] = {
	{ "buttons", "a", tablet_unstable_v2_types + 0 },
	{ "ring", "n", tablet_unstable_v2_types + 15 },
	{ "strip", "n", tablet_unstable_v2_types + 16 },
	{ "modes", "u", tablet_unstable_v2_types + 0 },
	{ "done", "", tablet_unstable_v2_types + 0 },
	{ "mode_switch", "uuu", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_pad_group_v2_interface = {
	"zwp_tablet_pad_group_v2", 1,
	1, zwp_tablet_pad_group_v2_requests,
	6, zwp_tablet_pad_group_v2_events,
};

static const struct wl_message zwp_tablet_pad_v2_requests[] = {
	{ "set_feedback", "usu", tablet_unstable_v2_types + 0 },
	{ "destroy", "", tablet_unstable_v2_types + 0 },
};

static const struct wl_message zwp_tablet_pad_v2_events[] = {
	{ "group", "n", tablet_unstable_v2_types + 17 },
	{ "path", "s", tablet_unstable_v2_types + 0 },
	{ "buttons", "u", tablet_unstable_v2_types + 0 },
	{ "done", "", tablet_unstable_v2_types + 0 },
	{ "button", "uuu", tablet_unstable_v2_types + 0 },
	{ "enter", "uoo", tablet_unstable_v2_types + 18 },
	{ "leave", "uo", tablet_unstable_v2_types + 21 },
	{ "removed", "", tablet_unstable_v2_types + 0 },
};

WL_PRIVATE const struct wl_interface zwp_tablet_pad_v2_interface = {
	"zwp_tablet_pad_v2", 1,
	2, zwp_tablet_pad_v2_requests,
	8, zwp_tablet_pad_v2_events,
};

