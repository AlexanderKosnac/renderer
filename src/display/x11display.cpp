#include <X11/Xlib.h>

#include <iostream>
#include <map>
#include <list>
#include <cstdlib>
#include <cfloat>

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
    image = new unsigned char[width*height*4];
    zbuffer = new float[width*height];

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

float DisplayX11::getWidth() {
    return width;
}

float DisplayX11::getHeight() {
    return height;
}

void DisplayX11::setWindowTitle(const std::string& s) {
    XStoreName(display, window, s.c_str());
}

void DisplayX11::handleEvent(XEvent& event) {
    CallbackType cbt = eventMapping[event.type];
    if (!cbt) return;

    for (const auto& callback : listeners[cbt]) {
        callback(event);
    }
}

void DisplayX11::update() {
    const int depth = 24;
    const int pad = 32;

    XImage* ximg = XCreateImage(display, DefaultVisual(display, screen), depth, ZPixmap, 0, reinterpret_cast<char*>(image), width, height, pad, 0);
    XPutImage(display, window, DefaultGC(display, screen), ximg, 0, 0, 0, 0, width, height);

    XEvent event;
    XNextEvent(display, &event);
    handleEvent(event);
}

void DisplayX11::setPixel(int x, int y, float z, const math::vec3& color) {
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    int i = y * width + x;
    if (z > zbuffer[i]) return; // Smaller z means closer to the camera.
    zbuffer[i] = z;
    int ii = i * 4;
    image[ii+0] = color.z;
    image[ii+1] = color.y;
    image[ii+2] = color.x;
    image[ii+3] = 255;
}

void DisplayX11::clear() {
    const int m = width*height*4;
    for (int i=0; i<m; ++i) {
        image[i] = 0;
    }
}

void DisplayX11::clearZBuffer() {
    const int m = width*height;
    for (int i=0; i<m; ++i) {
        zbuffer[i] = 1.0f;
    }
}