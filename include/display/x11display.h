#ifndef X11_DISPLAY
#define X11_DISPLAY

#include <X11/Xlib.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "display/callbacktypes.h"
#include "display/framebuffer.h"

class DisplayX11 {
  private:
    Display *display;
    Window window;
    int screen;

    int width;
    int height;

    std::vector<unsigned char> windowBuffer;

    std::map<CallbackType, std::vector<std::function<void(XEvent &)>>> listeners;

  public:
    DisplayX11(int width, int height);

    ~DisplayX11();

    void addListener(CallbackType type, std::function<void(XEvent &)> callbackFn);

    int getWidth();
    int getHeight();

    void setDimensions(int width, int height);

    void setWindowTitle(const std::string &s);

    void handleEvent(XEvent &event);

    void present(const Framebuffer &fb);
    void pollEvents();
    void resizeWindow(int w, int h);
};

#endif