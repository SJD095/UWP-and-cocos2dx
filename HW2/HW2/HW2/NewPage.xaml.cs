using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
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
using Windows.UI.Popups;

namespace HW2
{
        public sealed partial class NewPage : Page
        {
            public NewPage()
            {
                this.InitializeComponent();
                var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
                viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
                viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
            }

            protected override void OnNavigatedTo(NavigationEventArgs e)
            {
                Frame rootFrame = Window.Current.Content as Frame;

                if (rootFrame.CanGoBack)
                {
                    SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                        AppViewBackButtonVisibility.Visible;
                }
                else
                {
                    SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility =
                        AppViewBackButtonVisibility.Collapsed;
                }
            }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            if(Title.Text != "")
            {
                Title.Text = "";
            }

            if(Details.Text != "")
            {
                Details.Text = "";
            }

            Duedate.Date = DateTimeOffset.Now;
        }

        private void createButton_Click(object sender, RoutedEventArgs e)
        {
            if (Title.Text == "")
            {
                var i = new MessageDialog("Title is empty").ShowAsync();
            }

            if (Details.Text == "")
            {
                var i = new MessageDialog("Details is empty").ShowAsync();
            }

            if ((Duedate.Date.ToUnixTimeSeconds()) +86400 < (DateTimeOffset.Now.ToUnixTimeSeconds()))
            {
                var i = new MessageDialog("illegal date").ShowAsync();
            }
        }

        private void SelectPictureButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
