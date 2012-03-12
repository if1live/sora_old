using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;

namespace sora_tool
{
    class Sora
    {
        [DllImport("sora.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static float Vec2f_testFunc(float x, float y);

        [DllImport("sora.dll", CallingConvention = CallingConvention.Cdecl)]
        extern public static void SORA_setup_graphics(int w, int h);

        [DllImport("sora.dll")]
        extern public static void SORA_draw_frame();

        [DllImport("sora.dll")]
        extern public static void SORA_init_gl_env();
    }

}
