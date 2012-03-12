/*******************************************************
 *
 * Created by: Alexander Puzynia aka WerWolf
 * Created: 01.05.2008 21:43
 *
 * File: WindowsOpenGLNative.cs
 * Remarks:
 * 
 * History:
 *   01.05.2008 21:43 - Create Wireframe
 *
 *******************************************************/

using System;
using System.Runtime.InteropServices;

namespace ManagedOpenGL
{
    public static class WindowsOpenGLNative
    {
        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern IntPtr wglGetCurrentContext();

        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern IntPtr wglGetCurrentDC();

        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern IntPtr wglCreateContext(IntPtr hdc);

        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern bool wglMakeCurrent(IntPtr hdc, IntPtr hglrc);

        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern Int32 wglDeleteContext(IntPtr hglrc);

        [DllImport("opengl32.dll", SetLastError = true)]
        public static extern IntPtr wglGetProcAddress([MarshalAs(UnmanagedType.LPStr)]string functionName);

        [DllImport("gdi32.dll", SetLastError = true)]
        public static extern int ChoosePixelFormat(IntPtr hdc, ref PixelFormatDescriptor ppfd);

        [DllImport("gdi32.dll", SetLastError = true)]
        public static extern Int32 DescribePixelFormat(IntPtr hdc, int iPixelFormat, uint nbytes, ref PixelFormatDescriptor ppfd);

        [DllImport("gdi32.dll", SetLastError = true)]
        public static extern bool SetPixelFormat(IntPtr hdc, int iPixelFormat, ref PixelFormatDescriptor ppfd);

        [DllImport("gdi32.dll", SetLastError = true)]
        public static extern uint GetPixelFormat(IntPtr hdc);

        [DllImport("gdi32.dll", SetLastError = true)]
        public static extern bool SwapBuffers(IntPtr hdc);

        [DllImport("user32.dll")]
        public static extern IntPtr GetDC(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern Int32 ReleaseDC(IntPtr hWnd, IntPtr hDC);

        [DllImport("glu32.dll")]
        public static extern void gluPerspective(double fovy, double aspect, double zNear, double zFar);
    }
}