#include <X11/X.h>
#if 1 //defined(FM_PLATFORM_LINUX)

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include "FM/Platfrom/Window/Window.hpp"
#include "FM/Core/Log.hpp"
#include "FM/Core/Event.hpp"

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

    bool focus;
    fm::Vec2i size;
    bool open;

#if defined(FM_RENDER_API_OPENGL)
    GLXFBConfig best_fbc_config;
    GLXContext context;
#endif
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

#if defined(FM_RENDER_API_OPENGL)
        //// GLX ////        
        GLint major_glx, minor_glx = 0;
        glXQueryVersion(window_context->display, &major_glx, &minor_glx);
        if (major_glx <= 1 && minor_glx < 2) {
            FM_LOG_ERROR("GLX 1.2 or greater is required");
            XCloseDisplay(window_context->display);
        }

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
            FM_LOG_ERROR("Failed to retrieve framebuffer");
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

        window_context->best_fbc_config = fbc[ best_fbc ];
        XFree( fbc );

        XVisualInfo* visual = glXGetVisualFromFBConfig(window_context->display, window_context->best_fbc_config);
        //// GLX ////
#endif
        if (visual == 0) 
        {
            FM_LOG_ERROR("Could not create correct visual window");
            return;
        }

        XSetWindowAttributes attar;
        
        attar.event_mask = EnterWindowMask | LeaveWindowMask | ExposureMask |
            StructureNotifyMask | FocusChangeMask |
            KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
        attar.colormap = XCreateColormap(window_context->display, RootWindow(window_context->display, visual->screen), visual->visual, AllocNone);
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
        
        window_context->open = true; 
    }

    void Window::Shutdown()
    {
        WindowContext* window_context = (WindowContext*)m_context;
        XAutoRepeatOn(window_context->display);  
        XDestroyWindow(window_context->display, window_context->window);
#if defined(FM_RENDER_API_OPENGL)
        glXDestroyContext(window_context->display, window_context->context);        
#endif
        XCloseDisplay(window_context->display);
        
        delete window_context;
    }

    bool Window::PollEvent(Event& event)
    {
        WindowContext* window_context = (WindowContext*)m_context;
        
        if (XPending(window_context->display) == 0) return false;

        XEvent xevent;
        XNextEvent(window_context->display, &xevent);
        switch (xevent.type)
        {
            case ClientMessage:
                if (xevent.xclient.data.l[0] == window_context->delete_window) 
                {
                    event.type = EVENT_WINDOW_DISTROY;
                    event.window.type = EVENT_WINDOW_DISTROY;
                    window_context->open = false; 
                }
                break;

            case Expose:
                event.type = EVENT_WINDOW_RESIZE;
                event.window.type = EVENT_WINDOW_RESIZE;
                event.window.size = { xevent.xexpose.width, xevent.xexpose.height };
                window_context->size = { xevent.xexpose.width, xevent.xexpose.height };
                break;

            case ConfigureNotify:
                event.type = EVENT_WINDOW_MOTION;
                event.window.type = EVENT_WINDOW_MOTION;
                event.window.position = { xevent.xconfigure.x, xevent.xconfigure.y };
                event.window.size = { xevent.xconfigure.width, xevent.xconfigure.height };
                break;

            case FocusIn:
            case FocusOut:
                {
                    bool focus = xevent.xfocus.type == FocusIn;
                    event.type = focus ? EVENT_WINDOW_FOCUS : EVENT_WINDOW_UNFOCUS;
                    event.window.type = event.type;
                    event.window.focus = focus;
                    window_context->focus = focus;
                }
                break;


            case KeyPress:
            case KeyRelease:
                {
                    bool press = xevent.xkey.type == KeyPress;

                    KeySym xkey = XKeycodeToKeysym(window_context->display, xevent.xkey.keycode, 0);
                    KeyboardKey key = (KeyboardKey)KeySymToKeyboradKey((int)xkey);

                    event.type = press ? EVENT_KEYBOARD_KEY_PRESS : EVENT_KEYBOARD_KEY_RELEASE;
                    event.keyboard.type = event.type;
                    event.keyboard.press = press;
                    event.keyboard.key = key;
                }
                break;

            case ButtonPress:
            case ButtonRelease:
                {
                    bool press = xevent.xbutton.type == ButtonPress;

                    MouseButton button;
                    switch (xevent.xbutton.button) 
                    {
                        case Button1:
                            button = MouseButton::MOUSE_BUTTON_RIGHT;
                            event.type = press ? EVENT_MOUSE_BUTTON_PRESS : EVENT_MOUSE_BUTTON_RELEASE;
                            event.mouse.type = event.type;
                            event.mouse.press = press;
                            event.mouse.button = button;
                            break;

                        case Button2:
                            button = MouseButton::MOUSE_BUTTON_RIGHT;
                            event.type = press ? EVENT_MOUSE_BUTTON_PRESS : EVENT_MOUSE_BUTTON_RELEASE;
                            event.mouse.type = event.type;
                            event.mouse.press = press;
                            event.mouse.button = button;
                            break;

                        case Button3:
                            button = MouseButton::MOUSE_BUTTON_RIGHT;
                            event.type = press ? EVENT_MOUSE_BUTTON_PRESS : EVENT_MOUSE_BUTTON_RELEASE;
                            event.mouse.type = event.type;
                            event.mouse.press = press;
                            event.mouse.button = button;
                            break;

                        case Button4:
                            event.type = EVENT_MOUSE_SCROLL;
                            event.mouse.type = EVENT_MOUSE_SCROLL;
                            event.mouse.delta_scroll = (int)press * -1;
                            event.mouse.type = event.type;
                            break;

                        case Button5:
                            event.type = EVENT_MOUSE_SCROLL;
                            event.mouse.type = EVENT_MOUSE_SCROLL;
                            event.mouse.delta_scroll = (int)press * 1;
                            event.mouse.type = event.type;
                            break;
                    }
                    break;
                }

            case MotionNotify:
                event.type = EVENT_MOUSE_MOTION;
                event.mouse.type = EVENT_MOUSE_MOTION;
                event.mouse.position = { xevent.xmotion.x, xevent.xmotion.y };
                break;

            case LeaveNotify:
                event.type = EVENT_MOUSE_LEAVE;
                event.mouse.type = EVENT_MOUSE_LEAVE;
                event.mouse.focus = false;
                break;

            case EnterNotify:
                event.type = EVENT_MOUSE_ENTER;
                event.mouse.type = EVENT_MOUSE_ENTER;
                event.mouse.focus = true;
                break;
        }
        return true;

    }

    void Window::InitRenderContext()
    {
        WindowContext* window_context = (WindowContext*)m_context;

#if defined (FM_RENDER_API_OPENGL)
        const char *glxExts = glXQueryExtensionsString(window_context->display,  window_context->screen_id);
        
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
        glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        };

        window_context->context = glXCreateContextAttribsARB(window_context->display, window_context->best_fbc_config, 0, true, context_attribs);
        XSync(window_context->display, False);
        
        glXMakeCurrent(window_context->display, window_context->window, window_context->context);
        gladLoadGLLoader((GLADloadproc)glXGetProcAddress);
#endif
    }


    void Window::Display()
    {
        WindowContext* window_context = (WindowContext*)m_context;
        
        glXSwapBuffers(window_context->display, window_context->window);
    }
    
    int Window::KeySymToKeyboradKey(int key)
    {
        switch (key)
        {
            case XK_BackSpace:
                return KEYBOARD_KEY_BACKSPACE;
            case XK_Return:
                return KEYBOARD_KEY_ENTER;
            case XK_Tab:
                return KEYBOARD_KEY_TAB;

            case XK_Pause:
                return KEYBOARD_KEY_PAUSE;
            case XK_Caps_Lock:
                return KEYBOARD_KEY_CAPITAL;

            case XK_Escape:
                return KEYBOARD_KEY_ESCAPE;

            case XK_Mode_switch:
                return KEYBOARD_KEY_MODECHANGE;

            case XK_space:
                return KEYBOARD_KEY_SPACE;
            case XK_Prior:
                return KEYBOARD_KEY_PAGEUP;
            case XK_Next:
                return KEYBOARD_KEY_PAGEDOWN;
            case XK_End:
                return KEYBOARD_KEY_END;
            case XK_Home:
                return KEYBOARD_KEY_HOME;
            case XK_Left:
                return KEYBOARD_KEY_LEFT;
            case XK_Up:
                return KEYBOARD_KEY_UP;
            case XK_Right:
                return KEYBOARD_KEY_RIGHT;
            case XK_Down:
                return KEYBOARD_KEY_DOWN;
            case XK_Select:
                return KEYBOARD_KEY_SELECT;
            case XK_Print:
                return KEYBOARD_KEY_PRINT;
            case XK_Execute:
                return KEYBOARD_KEY_EXECUTE;
                // case XK_snapshot: return KEYBOARD_KEY_SNAPSHOT; // not supported
            case XK_Insert:
                return KEYBOARD_KEY_INSERT;
            case XK_Delete:
                return KEYBOARD_KEY_DELETE;
            case XK_Help:
                return KEYBOARD_KEY_HELP;

            case XK_Meta_L:
                return KEYBOARD_KEY_LSUPER; // TODO: not sure this is right
            case XK_Meta_R:
                return KEYBOARD_KEY_RSUPER;
                // case XK_apps: return KEYBOARD_KEY_APPS; // not supported

                // case XK_sleep: return KEYBOARD_KEY_SLEEP; //not supported

            case XK_KP_0:
                return KEYBOARD_KEY_NUMPAD0;
            case XK_KP_1:
                return KEYBOARD_KEY_NUMPAD1;
            case XK_KP_2:
                return KEYBOARD_KEY_NUMPAD2;
            case XK_KP_3:
                return KEYBOARD_KEY_NUMPAD3;
            case XK_KP_4:
                return KEYBOARD_KEY_NUMPAD4;
            case XK_KP_5:
                return KEYBOARD_KEY_NUMPAD5;
            case XK_KP_6:
                return KEYBOARD_KEY_NUMPAD6;
            case XK_KP_7:
                return KEYBOARD_KEY_NUMPAD7;
            case XK_KP_8:
                return KEYBOARD_KEY_NUMPAD8;
            case XK_KP_9:
                return KEYBOARD_KEY_NUMPAD9;
            case XK_multiply:
                return KEYBOARD_KEY_MULTIPLY;
            case XK_KP_Add:
                return KEYBOARD_KEY_ADD;
            case XK_KP_Separator:
                return KEYBOARD_KEY_SEPARATOR;
            case XK_KP_Subtract:
                return KEYBOARD_KEY_SUBTRACT;
            case XK_KP_Decimal:
                return KEYBOARD_KEY_DECIMAL;
            case XK_KP_Divide:
                return KEYBOARD_KEY_DIVIDE;
            case XK_F1:
                return KEYBOARD_KEY_F1;
            case XK_F2:
                return KEYBOARD_KEY_F2;
            case XK_F3:
                return KEYBOARD_KEY_F3;
            case XK_F4:
                return KEYBOARD_KEY_F4;
            case XK_F5:
                return KEYBOARD_KEY_F5;
            case XK_F6:
                return KEYBOARD_KEY_F6;
            case XK_F7:
                return KEYBOARD_KEY_F7;
            case XK_F8:
                return KEYBOARD_KEY_F8;
            case XK_F9:
                return KEYBOARD_KEY_F9;
            case XK_F10:
                return KEYBOARD_KEY_F10;
            case XK_F11:
                return KEYBOARD_KEY_F11;
            case XK_F12:
                return KEYBOARD_KEY_F12;
            case XK_F13:
                return KEYBOARD_KEY_F13;
            case XK_F14:
                return KEYBOARD_KEY_F14;
            case XK_F15:
                return KEYBOARD_KEY_F15;
            case XK_F16:
                return KEYBOARD_KEY_F16;
            case XK_F17:
                return KEYBOARD_KEY_F17;
            case XK_F18:
                return KEYBOARD_KEY_F18;
            case XK_F19:
                return KEYBOARD_KEY_F19;
            case XK_F20:
                return KEYBOARD_KEY_F20;
            case XK_F21:
                return KEYBOARD_KEY_F21;
            case XK_F22:
                return KEYBOARD_KEY_F22;
            case XK_F23:
                return KEYBOARD_KEY_F23;
            case XK_F24:
                return KEYBOARD_KEY_F24;

            case XK_Num_Lock:
                return KEYBOARD_KEY_NUMLOCK;
            case XK_Scroll_Lock:
                return KEYBOARD_KEY_SCROLL;

            case XK_KP_Equal:
                return KEYBOARD_KEY_NUMPAD_EQUAL;

            case XK_Shift_L:
                return KEYBOARD_KEY_LSHIFT;
            case XK_Shift_R:
                return KEYBOARD_KEY_RSHIFT;
            case XK_Control_L:
                return KEYBOARD_KEY_LCONTROL;
            case XK_Control_R:
                return KEYBOARD_KEY_RCONTROL;
            case XK_Alt_L:
                return KEYBOARD_KEY_LALT;
            case XK_Alt_R:
                return KEYBOARD_KEY_RALT;

            case XK_semicolon:
                return KEYBOARD_KEY_SEMICOLON;
            case XK_plus:
                return KEYBOARD_KEY_EQUAL;
            case XK_comma:
                return KEYBOARD_KEY_COMMA;
            case XK_minus:
                return KEYBOARD_KEY_MINUS;
            case XK_period:
                return KEYBOARD_KEY_PERIOD;
            case XK_slash:
                return KEYBOARD_KEY_SLASH;
            case XK_grave:
                return KEYBOARD_KEY_GRAVE;

            case XK_0:
                return KEYBOARD_KEY_0;
            case XK_1:
                return KEYBOARD_KEY_1;
            case XK_2:
                return KEYBOARD_KEY_2;
            case XK_3:
                return KEYBOARD_KEY_3;
            case XK_4:
                return KEYBOARD_KEY_4;
            case XK_5:
                return KEYBOARD_KEY_5;
            case XK_6:
                return KEYBOARD_KEY_6;
            case XK_7:
                return KEYBOARD_KEY_7;
            case XK_8:
                return KEYBOARD_KEY_8;
            case XK_9:
                return KEYBOARD_KEY_9;

            case XK_a:
            case XK_A:
                return KEYBOARD_KEY_A;
            case XK_b:
            case XK_B:
                return KEYBOARD_KEY_B;
            case XK_c:
            case XK_C:
                return KEYBOARD_KEY_C;
            case XK_d:
            case XK_D:
                return KEYBOARD_KEY_D;
            case XK_e:
            case XK_E:
                return KEYBOARD_KEY_E;
            case XK_f:
            case XK_F:
                return KEYBOARD_KEY_F;
            case XK_g:
            case XK_G:
                return KEYBOARD_KEY_G;
            case XK_h:
            case XK_H:
                return KEYBOARD_KEY_H;
            case XK_i:
            case XK_I:
                return KEYBOARD_KEY_I;
            case XK_j:
            case XK_J:
                return KEYBOARD_KEY_J;
            case XK_k:
            case XK_K:
                return KEYBOARD_KEY_K;
            case XK_l:
            case XK_L:
                return KEYBOARD_KEY_L;
            case XK_m:
            case XK_M:
                return KEYBOARD_KEY_M;
            case XK_n:
            case XK_N:
                return KEYBOARD_KEY_N;
            case XK_o:
            case XK_O:
                return KEYBOARD_KEY_O;
            case XK_p:
            case XK_P:
                return KEYBOARD_KEY_P;
            case XK_q:
            case XK_Q:
                return KEYBOARD_KEY_Q;
            case XK_r:
            case XK_R:
                return KEYBOARD_KEY_R;
            case XK_s:
            case XK_S:
                return KEYBOARD_KEY_S;
            case XK_t:
            case XK_T:
                return KEYBOARD_KEY_T;
            case XK_u:
            case XK_U:
                return KEYBOARD_KEY_U;
            case XK_v:
            case XK_V:
                return KEYBOARD_KEY_V;
            case XK_w:
            case XK_W:
                return KEYBOARD_KEY_W;
            case XK_x:
            case XK_X:
                return KEYBOARD_KEY_X;
            case XK_y:
            case XK_Y:
                return KEYBOARD_KEY_Y;
            case XK_z:
            case XK_Z:
                return KEYBOARD_KEY_Z;

            default:
                return 0;
        }    
    }

}


#endif
