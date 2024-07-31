#if defined(FM_PLATFORM_LINUX)

#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include "FM/Platfrom/Window/Window.hpp"
#include "FM/Core/Log.hpp"

namespace fm
{
    struct WindowContext
    {
        Display* display;        
        xcb_connection_t* connection;
        xcb_screen_t* screen;
        xcb_window_t window;
    };

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

        XAutoRepeatOff(window_context->display);
        
        window_context->connection = XGetXCBConnection(window_context->display);

        if (xcb_connection_has_error(window_context->connection))
        {
            FM_LOG_ERROR("faild to connect to x server");
            return;
        }

        xcb_screen_iterator_t iter;
        int screen_nbr = 0;
        iter = xcb_setup_roots_iterator(xcb_get_setup(window_context->connection));
        for (int32_t i = screen_nbr; i > 0; i--)
        {
            xcb_screen_next(&iter);
        }

        window_context->screen = iter.data;    
        
        window_context->window = xcb_generate_id(window_context->connection);

        uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        uint32_t event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
            XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | 
            XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        uint32_t value_array[] = {window_context->screen->black_pixel, event_values};

        xcb_void_cookie_t cookie = xcb_create_window(
            window_context->connection,
            XCB_COPY_FROM_PARENT,
            window_context->window, 
            window_context->screen->root, 
            position.x, 
            position.y, 
            size.x, 
            size.y, 
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT, 
            window_context->screen->root_visual, 
            event_mask,
            value_array
        );

        xcb_change_property(window_context->connection, 
            XCB_PROP_MODE_REPLACE,
            window_context->window,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            8,
            strlen(name),
            name
        );

        xcb_map_window(window_context->connection, window_context->window);

        int32_t result = xcb_flush(window_context->connection);
        if (result <= 0)
        {
            FM_LOG_ERROR("faild to flush window");
        }
    }

    void Window::Shutdown()
    {
        WindowContext* window_context = (WindowContext*)m_context;

        xcb_destroy_window(window_context->connection, window_context->window);
    }

    void Window::PollEvent()
    {
        WindowContext* window_context = (WindowContext*)m_context;

        xcb_generic_event_t *event;
        while ((event = xcb_wait_for_event(window_context->connection)))
        {
            switch (event->response_type & ~0x80) 
            {
            case XCB_EXPOSE:
            {
                xcb_expose_event_t* expose_event = (xcb_expose_event_t*)event;
                FM_LOG_INFO("expose: x:{0}, y:{1}, w{2}, h{3}", 
                    expose_event->x, expose_event->y, expose_event->width, expose_event->height);
                break;
            }
            case XCB_KEY_PRESS:
            {
                xcb_key_press_event_t* key_press_event = (xcb_key_press_event_t*)event;
                FM_LOG_INFO("key press: key:{0}", 
                    key_press_event->detail);
                break;
            }
            case XCB_KEY_RELEASE:
            {
                xcb_key_release_event_t* key_release_event = (xcb_key_release_event_t*)event;
                FM_LOG_INFO("key release: key:{0}", 
                    key_release_event->detail);
                break;
            }
            case XCB_BUTTON_PRESS:
            {
                xcb_button_press_event_t* button_press_event = (xcb_button_press_event_t*)event;
                FM_LOG_INFO("key press: key:{0}", 
                    button_press_event->detail);
                break;
            }
            case XCB_BUTTON_RELEASE:
            {
                xcb_button_release_event_t* button_release_event = (xcb_button_release_event_t*)event;
                FM_LOG_INFO("key release: key:{0}", 
                    button_release_event->detail);
                break;
            }
            };
        }
    }
}


#endif
