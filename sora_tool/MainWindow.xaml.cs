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
            
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            WindowsFormsHost host = new WindowsFormsHost();
            OpenGLView child = new OpenGLView();
            windowsFormsHost1.Child = child;
            //System.Windows.Forms.Button btn = new System.Windows.Forms.Button();
            //btn.Text = "asdf";
            //windowsFormsHost1.Child = btn;
        }

        private void button1_Click_1(object sender, RoutedEventArgs e)
        {
            //OpenGLForm form = new OpenGLForm();
            //form.ClientSize = new System.Drawing.Size(300, 300);
            //form.ShowDialog();
        }
   }
}