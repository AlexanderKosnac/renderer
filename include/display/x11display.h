#ifndef X11_DISPLAY
#define X11_DISPLAY

#include <map>
#include <vector>
#include <functional>
#include <string>

#include <X11/Xlib.h>

#include "display/callbacktypes.h"

class DisplayX11 {
private:
    Display* display;
    Window window;
    int screen;

    int width;
    int height;
    unsigned char* image;
    float* zbuffer;

    std::map<CallbackType, std::vector<std::function<void(XEvent&)>>> listeners;

public:
    DisplayX11(int width, int height);

    ~DisplayX11();

    void addListener(CallbackType type, std::function<void(XEvent&)> callbackFn);

    float getWidth();
    float getHeight();

    void setWindowTitle(const std::string& s);

    void handleEvent(XEvent &event);
    void update();
    void setPixel(int x, int y, float z, math::vec3& color);
    void clear();
    void clearZBuffer();
};

#endif