﻿using System;
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

namespace HW2
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
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

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(NewPage), "");
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            Line1.Stretch = Stretch.Fill;
        }

        private void CheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            Line1.Stretch = Stretch.None;
        }

        private void CheckBox_Checked_1(object sender, RoutedEventArgs e)
        {
            Line2.Stretch = Stretch.Fill;
        }

        private void CheckBox_Unchecked_1(object sender, RoutedEventArgs e)
        {
            Line2.Stretch = Stretch.None;
        }
    }
}