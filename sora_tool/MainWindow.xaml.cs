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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using ManagedOpenGL;
using System.ComponentModel;
using System.Windows.Forms.Integration;

using ManagedOpenGL.Engine.Windows;

namespace sora_tool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //OpenGLForm glForm_;
        public MainWindow()
        {
            InitializeComponent();
            
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            float a = 1;
            float b = 2;
            float c = Sora.Vec2f_testFunc(a, b);
            System.Console.WriteLine(c);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Create the interop host control.
            System.Windows.Forms.Integration.WindowsFormsHost host =
                new System.Windows.Forms.Integration.WindowsFormsHost();

            /*
            // Create the MaskedTextBox control.
            MaskedTextBox mtbDate = new MaskedTextBox("00/00/0000");
            // Assign the MaskedTextBox control as the host control's child.
            host.Child = mtbDate;
             */
         
            /*
            OpenGLForm form = new OpenGLForm();
            host.Child = form;
            

            // Add the interop host control to the Grid
            // control's collection of child controls.
            this.grid1.Children.Add(host);
             * */
            
        }

        private void button1_Click_1(object sender, RoutedEventArgs e)
        {
            OpenGLForm form = new OpenGLForm();
            form.ClientSize = new System.Drawing.Size(300, 300);

            form.ShowDialog();
        }
   }
}
