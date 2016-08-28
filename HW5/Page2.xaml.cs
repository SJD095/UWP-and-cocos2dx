using HW2.ViewModels;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace HW2
{
    public sealed partial class Page2 : Page
    {
        public Page2()
        {
            this.InitializeComponent();

            ViewModel = new TheViewModel();
            DataContext = ViewModel;

            ViewModel.LoadData();
        }

        TheViewModel ViewModel { get; set; }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            ((App)App.Current).BackRequested += Page2_BackRequested;

            if (e.NavigationMode == NavigationMode.New)
            {
                ApplicationData.Current.LocalSettings.Values.Remove("TheWorkInProgress");
            }
            else
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("TheWorkInProgress"))
                {
                    var composite = ApplicationData.Current.LocalSettings.Values["TheWorkInProgress"] as ApplicationDataCompositeValue;
                    textBox1.Text = (string)composite["Field1"];
                    textBox2.Text = (string)composite["Field2"];
                    
                    ApplicationData.Current.LocalSettings.Values.Remove("TheWorkInProgress");
                }
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            ((App)App.Current).BackRequested -= Page2_BackRequested;

            bool suspending = ((App)App.Current).IsSuspending;
            if (suspending)
            {
                var composite = new ApplicationDataCompositeValue();
                composite["Field1"] = textBox1.Text;
                composite["Field2"] = textBox2.Text;
                ApplicationData.Current.LocalSettings.Values["TheWorkInProgress"] = composite;
            }
        }

        private void Page2_BackRequested(object sender, BackRequestedEventArgs e)
        {
            ViewModel.SaveData();
        }
    }
}
