using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.ComponentModel;
using PropertyTools.DataAnnotations;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Media;

using SoraToolLibrary;
using sora;

namespace sora_tool
{
    public class MainViewModel
    {
        object Model { get; set; }
        public object SelectedItem { get; set; }
        /*
        public MainViewModel()
        {
            Model = new TestSimpleTypes2();
            SelectedItem = Model;
        }
         * */

        public MainViewModel(RunaView glView)
        {
            Model = new Material(glView);
            this.SelectedItem = Model;
        }       
    }
}
