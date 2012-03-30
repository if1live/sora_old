using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ManagedOpenGL;
using ManagedOpenGL.Engine.Windows;

namespace sora
{
    public partial class OpenGLView : PictureBox
    {
        private GLView view;
        private bool initialized = false;
        private Timer timer;

        #region Fields
        private IntPtr hDC = IntPtr.Zero;
        private IntPtr hRC = IntPtr.Zero;
        private readonly HiResTimer hiResTimer = new HiResTimer();
        #endregion

        public OpenGLView(GLView gl_view)
        {
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);
            //this.SetStyle(ControlStyles.DoubleBuffer, true);

            InitializeComponent();

            this.view = gl_view;
            //System.Windows.Forms.Application.Idle += new EventHandler(UpdateElapsed);
            timer = new Timer();
            timer.Interval = (int)(1000 / 60.0f);
            timer.Tick += new EventHandler(UpdateElapsed);
            timer.Start();
        }

        protected void OnLoad(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnLoad");

            //base.OnLoad(e);
            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

            this.Init();
            this.InitGL();
        }

        protected void OnClosed(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnClosed");
            //base.OnClosed(e);

            WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

            this.DeInit();
        }

        private void OnResize(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnResize");
            //base.OnResize(e);
            this.Invalidate();
        }

        protected void OnResizeBegin(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnResizeBegin");
            //base.OnResizeBegin(e);
            hiResTimer.Paused = true;
        }

        protected void OnResizeEnd(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnResizeEnd");
            //base.OnResizeEnd(e);
            hiResTimer.Paused = false;
        }

        protected void OnSizeChanged(object sender, EventArgs e)
        {
            //System.Console.WriteLine("OnSizeChanged");
            //base.OnSizeChanged(e);

            if (initialized == false)
            {
                this.Init();
                this.InitGL();
            }
        }

        private void InitGL()
        {
            System.Console.WriteLine("Init GL");

            if (!WindowsOpenGLNative.wglMakeCurrent(this.hDC, this.hRC))
                throw new Win32Exception(Marshal.GetLastWin32Error());

            view.InitGLEnv();    //glew
            this.Invalidate();

            //아래가 호출되면 생성된 gl ctx를 다시 박살내버려서 좃망
            //WindowsOpenGLNative.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

            view.SetupGraphics(this.Size.Width, this.Size.Height);

            AfterInitGL();
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

            initialized = true;

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

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            //base.OnPaintBackground(e);
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
            view.SetWindowSize(this.Size.Width, this.Size.Height);
            view.DrawFrame();
            //view.TestDraw(this.Size.Width, this.Size.Height);
        }

        protected virtual void Update(float elapsed)
        {
        }
    }
}
