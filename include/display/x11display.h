#ifndef X11_DISPLAY
#define X11_DISPLAY

#include <map>
#include <vector>
#include <functional>

#include <X11/Xlib.h>

#include "display/callbacktypes.h"

class DisplayX11 {
private:
    Display* display;
    Window window;
    int screen;

    std::map<CallbackType, std::vector<std::function<void(XEvent&)>>> listeners;

public:
    DisplayX11(int width, int height);

    ~DisplayX11();

    void addListener(CallbackType type, std::function<void(XEvent&)> callbackFn);

    void handleEvent(XEvent &event);
    void update();
};

#endif