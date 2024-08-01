#include <X11/X.h>
#if defined(FM_PLATFORM_LINUX)

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include "FM/Platfrom/Window/Window.hpp"
#include "FM/Core/Log.hpp"

if defined(FM_RENDER_API_OPENGL)
    #include <GL/glx.h>
#endif


struct WindowContext
{
    Display* display;
    Screen* screen;
    Window window;
    int screen_id;
    Atom delete_window;
};

namespace fm
{

    Window::Window()
    {

    }

    Window::Window(fm::Vec2f position, fm::Vec2u size, const char* name)
    {
        Init(position, size, name);
    }

    Window::~Window()
    {

    }

    void Window::Init(fm::Vec2f position, fm::Vec2u size, const char* name)
    {
        m_context = new WindowContext;
        WindowContext* window_context = (WindowContext*)m_context; 
        
        window_context->display = XOpenDisplay(NULL);

        if (window_context->display == NULL)
        {
            FM_LOG_ERROR("faild to connect to x server");
            return;
        }

        XAutoRepeatOff(window_context->display);

        window_context->screen = DefaultScreenOfDisplay(window_context->display);
        window_context->screen_id = DefaultScreen(window_context->display);
        
        XSetWindowAttributes attar;
        
        attar.background_pixel = BlackPixel(window_context->display, window_context->screen_id);
        attar.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask | StructureNotifyMask;

        window_context->window = XCreateWindow(
            window_context->display, 
            RootWindowOfScreen(window_context->screen), 
            position.x, 
            position.y, 
            size.x, 
            size.y, 
            0, 
            CopyFromParent, 
            InputOutput, 
            CopyFromParent,
            CWBackPixel | CWEventMask,
            &attar
        );

        XStoreName(window_context->display, window_context->window, name);
             
        window_context->delete_window = XInternAtom(window_context->display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(window_context->display, window_context->window, &window_context->delete_window, 1);        
        
        XMapWindow(window_context->display, window_context->window);
    }

    void Window::Shutdown()
    {
        FM_LOG_INFO("Window Shuting Down");
        WindowContext* window_context = (WindowContext*)m_context;
        XAutoRepeatOn(window_context->display);  
        XDestroyWindow(window_context->display, window_context->window);
        
        XCloseDisplay(window_context->display);

        delete window_context;
    }

    bool Window::PollEvent()
    {
        WindowContext* window_context = (WindowContext*)m_context;

        // TODO: Event System
        XEvent event;
        XNextEvent(window_context->display, &event);

        switch (event.type)
        {
            case Expose:
                FM_LOG_INFO("Expose: x:{0}, y:{1}, w:{2}, h:{3}", event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
                break;

            case ClientMessage:
                if (event.xclient.data.l[0] == window_context->delete_window) {
                    return false; 
                }
                break;
        } 
        return true;
    }

    void Window::InitRenderContext()
    {
        WindowContext* window_context = (WindowContext*)m_context;
#if defined (FM_RENDER_API_OPENGL)

        GLint majorGLX, minorGLX = 0;
        glXQueryVersion(display, &majorGLX, &minorGLX);
        if (majorGLX <= 1 && minorGLX < 2) {
            FM_LOG_ERROR("GLX 1.2 or greater is required");
            XCloseDisplay(window_context->display);
            return 1;
        }

        FM_LOG_INFO("GLX Version: {0}.{1}", majorGLX, majorGLX);

        GLint attribs[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE,     24,
            GLX_STENCIL_SIZE,   8,
            GLX_RED_SIZE,       8,
            GLX_GREEN_SIZE,     8,
            GLX_BLUE_SIZE,      8,
            GLX_SAMPLE_BUFFERS, 0,
            GLX_SAMPLES,        0,
            None
        };
        XVisualInfo* visual = glXChooseVisual(window_context->display, window_context->screenId, attribs);

        if (visual == NULL) {
            FM_LOG_ERROR("Could not create correct visual window");
            XCloseDisplay(display);
            return;
        }

#endif
    }
}


#endif
