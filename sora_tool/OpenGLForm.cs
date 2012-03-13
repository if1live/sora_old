using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using sora_tool;

namespace ManagedOpenGL.Engine.Windows
{
    //public partial class OpenGLForm : Form
    public partial class OpenGLForm : Form
    {
        #region Fields
        private IntPtr hDC = IntPtr.Zero;
        private IntPtr hRC = IntPtr.Zero;
        private readonly HiResTimer hiResTimer = new HiResTimer();
        #endregion

        #region Constructors
        public OpenGLForm()
        {
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);

            this.InitializeComponent();

            this.ClientSize = new Size(300, 300);

            System.Windows.Forms.Application.Idle += new EventHandler(UpdateElapsed);
        }
        #endregion

        public Size WindowSize { get; set; }

        protected void OnLoad(object sender, EventArgs e)
        {
            //base.OnLoad(e);
            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

            this.Init();
            this.ClientSize = WindowSize;
            this.InitGL();
        }

        protected void OnClosed(object sender, EventArgs e)
        {
            //base.OnClosed(e);

            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

            this.DeInit();
        }

        private void OnResize(object sender, EventArgs e)
        {
            //base.OnResize(e);
            this.Invalidate();
        }

        protected void OnResizeBegin(object sender, EventArgs e)
        {
            //base.OnResizeBegin(e);
            hiResTimer.Paused = true;
        }

        protected void OnResizeEnd(object sender, EventArgs e)
        {
            //base.OnResizeEnd(e);
            hiResTimer.Paused = false;
        }

        protected void OnSizeChanged(object sender, EventArgs e)
        {
            //base.OnSizeChanged(e);
            this.Init();
            this.InitGL();
        }

        private void InitGL()
        {
            //Sora.Sora.SORA_init_gl_env();
            if (!WindowsOpenGLNative.wglMakeCurrent(this.hDC, this.hRC))
                throw new Win32Exception(Marshal.GetLastWin32Error());
            
            Sora.SORA_init_gl_env();    //glew

            this.Invalidate();

            //아래가 호출되면 생성된 gl ctx를 다시 박살내버려서 좃망
            //WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);
            Sora.SORA_setup_graphics(300, 300);
            
        }

        private void Init()
        {
            var pfd = new PixelFormatDescriptor();
            pfd.Initialize();

            this.hDC = WindowsOpenGLNative.GetDC(this.Handle);
            if (this.hDC == IntPtr.Zero)
                throw new Win32Exception(Marshal.GetLastWin32Error());

            var iPixelformat = WindowsOpenGLNative.ChoosePixelFormat(this.hDC, ref pfd);
            if (iPixelformat == 0)
                throw new Win32Exception(Marshal.GetLastWin32Error());

            // Set the pixel format
            if (!WindowsOpenGLNative.SetPixelFormat(this.hDC, iPixelformat, ref pfd))
                throw new Win32Exception(Marshal.GetLastWin32Error());

            // Create a new OpenGL rendering context
            this.hRC = WindowsOpenGLNative.wglCreateContext(this.hDC);
            if (this.hRC == IntPtr.Zero)
                throw new Win32Exception(Marshal.GetLastWin32Error());
        }

        private void DeInit()
        {
            if (this.hRC != IntPtr.Zero)
                WindowsOpenGLNative.wglDeleteContext(this.hRC);

            if (this.hDC != IntPtr.Zero)
                WindowsOpenGLNative.ReleaseDC(this.Handle, this.hDC);
        }

        private void AfterInitGL()
        {
            if (!WindowsOpenGLNative.wglMakeCurrent(this.hDC, this.hRC))
                throw new Win32Exception(Marshal.GetLastWin32Error());

            //Renderer.Initialized = true;

            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);
        }

        protected void OnPaint(object sender, PaintEventArgs e)
        {
            if (!WindowsOpenGLNative.wglMakeCurrent(this.hDC, this.hRC))
                throw new Win32Exception(Marshal.GetLastWin32Error());

            this.Draw();

            WindowsOpenGLNative.SwapBuffers(this.hDC);

            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);
        }

        internal void UpdateElapsed(object sender, EventArgs e)
        {
            if (hiResTimer.Paused) return;
            hiResTimer.Update();
            Update(hiResTimer.Elapsed);
            Invalidate();
        }

        protected virtual void Draw()
        {
            Sora.SORA_draw_frame();
            int a = 1;
            int b = a + 1;
        }

        protected virtual void Update(float elapsed)
        {
        }
    }
}