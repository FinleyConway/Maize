#pragma once

#include <SDL.h>

namespace Maize {

    enum class KeyCode
    {
        // alphabets
        A = SDLK_a,
        B = SDLK_b,
        C = SDLK_c,
        D = SDLK_d,
        E = SDLK_e,
        F = SDLK_f,
        G = SDLK_g,
        H = SDLK_h,
        I = SDLK_i,
        J = SDLK_j,
        K = SDLK_k,
        L = SDLK_l,
        M = SDLK_m,
        N = SDLK_n,
        O = SDLK_o,
        P = SDLK_p,
        Q = SDLK_q,
        R = SDLK_r,
        S = SDLK_s,
        T = SDLK_t,
        U = SDLK_u,
        V = SDLK_v,
        W = SDLK_w,
        X = SDLK_x,
        Y = SDLK_y,
        Z = SDLK_z,

        // numeric keys
        Num0 = SDLK_0,
        Num1 = SDLK_1,
        Num2 = SDLK_2,
        Num3 = SDLK_3,
        Num4 = SDLK_4,
        Num5 = SDLK_5,
        Num6 = SDLK_6,
        Num7 = SDLK_7,
        Num8 = SDLK_8,
        Num9 = SDLK_9,

        // function keys
        F1 = SDLK_F1,
        F2 = SDLK_F2,
        F3 = SDLK_F3,
        F4 = SDLK_F4,
        F5 = SDLK_F5,
        F6 = SDLK_F6,
        F7 = SDLK_F7,
        F8 = SDLK_F8,
        F9 = SDLK_F9,
        F10 = SDLK_F10,
        F11 = SDLK_F11,
        F12 = SDLK_F12,

        // arrow keys
        Up = SDLK_UP,
        Down = SDLK_DOWN,
        Left = SDLK_LEFT,
        Right = SDLK_RIGHT,

        // other keys
        Space = SDLK_SPACE,
        Enter = SDLK_RETURN,
        Escape = SDLK_ESCAPE,
        Tab = SDLK_TAB,
        Shift = SDLK_LSHIFT,
        Ctrl = SDLK_LCTRL,
        Alt = SDLK_LALT,
        Backspace = SDLK_BACKSPACE,
        Delete = SDLK_DELETE,
        Home = SDLK_HOME,
        End = SDLK_END,
        PageUp = SDLK_PAGEUP,
        PageDown = SDLK_PAGEDOWN,
        Insert = SDLK_INSERT,
        CapsLock = SDLK_CAPSLOCK,
        NumLock = SDLK_NUMLOCKCLEAR,
        ScrollLock = SDLK_SCROLLLOCK,
        PrintScreen = SDLK_PRINTSCREEN,
        Pause = SDLK_PAUSE,
        Menu = SDLK_MENU,
    };

}