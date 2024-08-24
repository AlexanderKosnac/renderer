#include <X11/Xlib.h>

#include <iostream>
#include <map>
#include <list>
#include <cstdlib>

#include "math.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

std::map<int, CallbackType> eventMapping = {
    {Expose, EXPOSE},
    {KeyPress, KEY_PRESS},
    {KeyRelease, KEY_RELEASE},
    {ButtonPress, BUTTON_PRESS},
    {ButtonRelease, BUTTON_RELEASE},
    {MotionNotify, MOUSE_MOTION},
};

DisplayX11::DisplayX11(int pWidth, int pHeight) {
    width = pWidth;
    height = pHeight;
    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Unable to open X display" << std::endl;
        return;
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, width, height, 1, WhitePixel(display, screen), BlackPixel(display, screen));

    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask);
    XMapWindow(display, window);
}

DisplayX11::~DisplayX11() {
    XCloseDisplay(display);
}

void DisplayX11::addListener(CallbackType type, std::function<void(XEvent&)> callbackFn) {
    listeners[type].push_back(callbackFn);
}

void DisplayX11::handleEvent(XEvent& event) {
    CallbackType cbt = eventMapping[event.type];
    if (!cbt) {
        return;
    }

    for (const auto& callback : listeners[cbt]) {
        callback(event);
    }
}

void DisplayX11::update() {
    XEvent event;
    XNextEvent(display, &event);
    handleEvent(event);
}
