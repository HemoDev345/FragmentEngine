#if defined(FM_PLATFORM_LINUX)

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include "FM/Platfrom/Window/Window.hpp"
#include "FM/Core/Log.hpp"

#if defined(FM_RENDER_API_OPENGL)
    #include <glad/glad.h>

    #include <GL/glx.h>
    
    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#endif


struct WindowContext
{
    Display* display;
    Screen* screen;
    Window window;
    int screen_id;
    Atom delete_window;

#if defined(FM_RENDER_API_OPENGL)
    GLXContext context;
#end

    Colormap cmap;
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
        
        GLint major_glx, minor_glx = 0;
        glXQueryVersion(window_context->display, &major_glx, &minor_glx);
        if (major_glx <= 1 && minor_glx < 2) {
            FM_LOG_ERROR("GLX 1.2 or greater is required");
            XCloseDisplay(window_context->display);
        }

        FM_LOG_INFO("GLX Version: {0}.{1}", major_glx, minor_glx);
		FM_LOG_INFO("GLX server version: " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_VERSION) << "\n";
		FM_LOG_INFO("GLX server vendoe: " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_VENDOR) << "\n";
		FM_LOG_INFO("GLX server extensions:\n\t " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_EXTENSIONS) << "\n";
        
        GLint attribs[] = {
            GLX_X_RENDERABLE    , True,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_RED_SIZE        , 8,
            GLX_GREEN_SIZE      , 8,
            GLX_BLUE_SIZE       , 8,
            GLX_ALPHA_SIZE      , 8,
            GLX_DEPTH_SIZE      , 24,
            GLX_STENCIL_SIZE    , 8,
            GLX_DOUBLEBUFFER    , True,
            //GLX_SAMPLE_BUFFERS  , 1,
            //GLX_SAMPLES         , 4,
            None
        };

        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(window_context->display, window_context->window, attribs, &fbcount);
        if (fbc == 0) 
        {
            FM_LOG_INFO("Failed to retrieve framebuffer.\n";
            return;
        }

        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
        for (int i = 0; i < fbcount; ++i)
        {
            XVisualInfo *vi = glXGetVisualFromFBConfig( window_context->display, fbc[i] );
            if ( vi != 0) 
            {
                int samp_buf, samples;
                glXGetFBConfigAttrib(window_context->display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                glXGetFBConfigAttrib(window_context->display, fbc[i], GLX_SAMPLES       , &samples  );
                //FM_LOG_INFO("  Matching fbconfig " << i << ", SAMPLE_BUFFERS = " << samp_buf << ", SAMPLES = " << samples << ".\n";

                if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) ) 
                {
                    best_fbc = i;
                    best_num_samp = samples;
                }
                if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                    worst_fbc = i;
                worst_num_samp = samples;
            }
            XFree( vi );
        }

        GLXFBConfig best_fbc_config = fbc[ best_fbc ];
        XFree( fbc );

        XVisualInfo* visual = glXGetVisualFromFBConfig(window_context->display, best_fbc_config);
        if (visual == 0) 
        {
            FM_LOG_INFO("Could not create correct visual window.\n";
            return;
        }

        XSetWindowAttributes attar;
        
        attar.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask | StructureNotifyMask;
        attar.colormap = window_context->cmap = XCreateColormap(window_context->display, RootWindow(window_context->display, visual->screen), visual->visual, AllocNone);
        attar.background_pixmap = None;

        window_context->window = XCreateWindow(
            window_context->display, 
            RootWindow(window_context->display, visual->screen),
            position.x, 
            position.y, 
            size.x, 
            size.y, 
            0, 
            visual->depth, 
            InputOutput, 
            visual->visual,
            CWColormap | CWEventMask,
            &attar
        );
        
        XStoreName(window_context->display, window_context->window, name);
             
        window_context->delete_window = XInternAtom(window_context->display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(window_context->display, window_context->window, &window_context->delete_window, 1);        
        
        XMapWindow(window_context->display, window_context->window);

        const char *glxExts = glXQueryExtensionsString(window_context->display,  window_context->screen_id);
        
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
        glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            None
        };


        window_context->context = glXCreateContextAttribsARB(window_context->display, best_fbc_config, 0, true, context_attribs);
        XSync(window_context->display, False);
        
        glXMakeCurrent(window_context->display, window_context->window, window_context->context );
        gladLoadGLLoader((GLADloadproc)glXGetProcAddress);
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

#if 1 //defined (FM_RENDER_API_OPENGL)
        GLint major_glx, minor_glx = 0;
        glXQueryVersion(window_context->display, &major_glx, &minor_glx);
        if (major_glx <= 1 && minor_glx < 2) {
            FM_LOG_ERROR("GLX 1.2 or greater is required");
            XCloseDisplay(window_context->display);
        }

        FM_LOG_INFO("GLX Version: {0}.{1}", major_glx, minor_glx);
		FM_LOG_INFO("GLX server version: " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_VERSION) << "\n";
		FM_LOG_INFO("GLX server vendoe: " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_VENDOR) << "\n";
		FM_LOG_INFO("GLX server extensions:\n\t " << glXQueryServerString(window_context->display, window_context->screen_id, GLX_EXTENSIONS) << "\n";
        
        

        GLint attribs[] = {
            GLX_X_RENDERABLE    , True,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_RED_SIZE        , 8,
            GLX_GREEN_SIZE      , 8,
            GLX_BLUE_SIZE       , 8,
            GLX_ALPHA_SIZE      , 8,
            GLX_DEPTH_SIZE      , 24,
            GLX_STENCIL_SIZE    , 8,
            GLX_DOUBLEBUFFER    , True,
            //GLX_SAMPLE_BUFFERS  , 1,
            //GLX_SAMPLES         , 4,
            None
        };

        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(window_context->display, window_context->window, attribs, &fbcount);
        if (fbc == 0) 
        {
            FM_LOG_INFO("Failed to retrieve framebuffer.\n";
            return;
        }

        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
        for (int i = 0; i < fbcount; ++i)
        {
            XVisualInfo *vi = glXGetVisualFromFBConfig( window_context->display, fbc[i] );
            if ( vi != 0) 
            {
                int samp_buf, samples;
                glXGetFBConfigAttrib(window_context->display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                glXGetFBConfigAttrib(window_context->display, fbc[i], GLX_SAMPLES       , &samples  );
                //FM_LOG_INFO("  Matching fbconfig " << i << ", SAMPLE_BUFFERS = " << samp_buf << ", SAMPLES = " << samples << ".\n";

                if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) ) 
                {
                    best_fbc = i;
                    best_num_samp = samples;
                }
                if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                    worst_fbc = i;
                worst_num_samp = samples;
            }
            XFree( vi );
        }

        GLXFBConfig best_fbc_config = fbc[ best_fbc ];
        XFree( fbc );

        XVisualInfo* visual = glXGetVisualFromFBConfig(window_context->display, best_fbc_config);
        if (visual == 0) 
        {
            FM_LOG_INFO("Could not create correct visual window.\n";
            XCloseDisplay(window_context->display);
        }

        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
        glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };
        
        const char *glxExts = glXQueryExtensionsString(window_context->display,  window_context->screen_id);
        
        window_context->context = glXCreateContextAttribsARB(window_context->display, best_fbc_config, 0, true, context_attribs);
        XSync(window_context->display, False);

        glXMakeCurrent(window_context->display, window_context->window, window_context->context);
#endif
    }


    void Window::Display()
    {
        WindowContext* window_context = (WindowContext*)m_context;
        
        glXSwapBuffers(window_context->display, window_context->window);
    }
}


#endif
