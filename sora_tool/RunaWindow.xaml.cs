using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using sora;
using System.ComponentModel;
using PropertyTools.DataAnnotations;

namespace sora_tool
{
   
    /// <summary>
    /// Interaction logic for RunaWindow.xaml
    /// </summary>
    public partial class RunaWindow : Window
    {
        RunaView glView = null;

        public RunaWindow()
        {
            InitializeComponent();
            glView = new RunaView();
            MainViewModel model = new MainViewModel(glView);
            //MainViewModel model = new MainViewModel();
            DataContext = model;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            OpenGLView child1 = new OpenGLView(glView);
            child1.MouseDown += child1_MouseDown;
            child1.MouseMove += child1_MouseMove;
            child1.MouseUp += child1_MouseUp;
            child1.MouseLeave += child1_MouseLeave;
            child1.Paint += child1_Paint;
            windowsFormsHost1.Child = child1;

            glContainer = child1;
        }

        float turnSpeed = 50.0f;
        bool camMove = false;
        int mouseX = 0;
        int mouseY = 0;
        float camRotateDeg = 0;
        OpenGLView glContainer = null;

        void child1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            //카메라 위치를 적절히 변경.
            if (camMove == true)
            {
                float dt = 1.0f / 60.0f;

                int width = glContainer.Size.Width;
                if (width / 2 > mouseX)
                {
                    //<==
                    camRotateDeg -= dt * turnSpeed;
                }
                else
                {
                    //==>
                    camRotateDeg += dt * turnSpeed;
                }
                glView.SetCamRotateDeg(camRotateDeg);
            }
        }

        void child1_MouseLeave(object sender, EventArgs e)
        {
            camMove = false;
        }

        void child1_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            camMove = false;
        }

        void child1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            mouseX = e.X;
            mouseY = e.Y;
        }

        void child1_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            camMove = true;
            mouseX = e.X;
            mouseY = e.Y;
        }

        private void FileExitClick(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
