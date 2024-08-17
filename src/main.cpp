#include <X11/Xlib.h>
#include <iostream>

int main() {
    // Open a connection to the X server
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Unable to open X display" << std::endl;
        return 1;
    }

    // Create a simple window
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 500, 500, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    // Select the type of events we are interested in
    XSelectInput(display, window, KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | ExposureMask);

    // Map (show) the window
    XMapWindow(display, window);

    // Event loop
    XEvent event;
    bool running = true;
    while (running) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                // Draw the window content (for example, a simple rectangle)
                XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 100, 100);
                XDrawPoint(display, window, DefaultGC(display, screen), 5, 5);
                break;

            case KeyPress:
                std::cout << "Key pressed: " << XLookupKeysym(&event.xkey, 0) << std::endl;
                break;

            case KeyRelease:
                std::cout << "Key released: " << XLookupKeysym(&event.xkey, 0) << std::endl;
                break;

            case ButtonPress:
                std::cout << "Mouse Button " << event.xbutton.button 
                          << " Pressed at (" << event.xbutton.x 
                          << ", " << event.xbutton.y << ")" << std::endl;
                break;

            case ButtonRelease:
                std::cout << "Mouse Button " << event.xbutton.button 
                          << " Released at (" << event.xbutton.x 
                          << ", " << event.xbutton.y << ")" << std::endl;
                break;

            case MotionNotify:
                std::cout << "Mouse Moved to (" 
                          << event.xmotion.x << ", " << event.xmotion.y << ")" 
                          << std::endl;
                break;

            default:
                break;
        }
    }

    // Close the connection to the X server
    XCloseDisplay(display);

    return 0;
}
