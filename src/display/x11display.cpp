#include "display/x11display.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cfloat>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <map>

#include "display/callbacktypes.h"
#include "display/framebuffer.h"
#include "math.h"

std::map<int, CallbackType> eventMapping = {
    {Expose, EXPOSE}, {KeyPress, KEY_PRESS}, {KeyRelease, KEY_RELEASE}, {ButtonPress, BUTTON_PRESS}, {ButtonRelease, BUTTON_RELEASE}, {MotionNotify, MOUSE_MOTION}, {ConfigureNotify, WINDOW_RESIZE},
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

    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | StructureNotifyMask);
    XMapWindow(display, window);
}

DisplayX11::~DisplayX11() { XCloseDisplay(display); }

void DisplayX11::addListener(CallbackType type, std::function<void(XEvent &)> callbackFn) { listeners[type].push_back(callbackFn); }

int DisplayX11::getWidth() { return width; }

int DisplayX11::getHeight() { return height; }

void DisplayX11::setDimensions(int w, int h) {
    width = w;
    height = h;
}

void DisplayX11::setWindowTitle(const std::string &s) { XStoreName(display, window, s.c_str()); }

void DisplayX11::handleEvent(XEvent &event) {
    CallbackType cbt = eventMapping[event.type];
    if (!cbt)
        return;

    for (const auto &callback : listeners[cbt]) {
        callback(event);
    }
}

void DisplayX11::pollEvents() {
    while (XPending(display)) {
        XEvent event;
        XNextEvent(display, &event);
        handleEvent(event);
    }
}

void DisplayX11::present(const Framebuffer &fb) {
    const int winW = width;
    const int winH = height;

    if (winW <= 0 || winH <= 0)
        return;

    if ((int)windowBuffer.size() != winW * winH * 4) {
        windowBuffer.resize(winW * winH * 4);
    }

    // Nearest-neighbor scaling
    const float sx = static_cast<float>(fb.width) / winW;
    const float sy = static_cast<float>(fb.height) / winH;

    for (int y = 0; y < winH; ++y) {
        int srcY = static_cast<int>(y * sy);
        if (srcY >= fb.height)
            srcY = fb.height - 1;

        for (int x = 0; x < winW; ++x) {
            int srcX = static_cast<int>(x * sx);
            if (srcX >= fb.width)
                srcX = fb.width - 1;

            const int dstI = (y * winW + x) * 4;
            const int srcI = (srcY * fb.width + srcX) * 4;

            windowBuffer[dstI + 0] = fb.color[srcI + 0];
            windowBuffer[dstI + 1] = fb.color[srcI + 1];
            windowBuffer[dstI + 2] = fb.color[srcI + 2];
            windowBuffer[dstI + 3] = 255;
        }
    }

    const int depth = 24;
    const int pad = 32;

    XImage *img = XCreateImage(display, DefaultVisual(display, screen), depth, ZPixmap, 0, reinterpret_cast<char *>(windowBuffer.data()), winW, winH, pad, 0);
    XPutImage(display, window, DefaultGC(display, screen), img, 0, 0, 0, 0, winW, winH);

    img->data = nullptr;
    XDestroyImage(img);
}

void DisplayX11::resizeWindow(int w, int h) {
    XResizeWindow(display, window, w, h);
    XFlush(display);
}