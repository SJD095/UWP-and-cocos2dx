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
using Windows.UI.Popups;
using HW2.ViewModels;
using SQLitePCL;

namespace HW2
{

        public sealed partial class NewPage : Page
        {
        TheViewModel ViewModel2 { get; set; }
        public NewPage()
            {
                this.InitializeComponent();
                var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
                viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
                viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;

            }

        private ViewModels.ItemViewModel ViewModel;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        { 
            if (e.NavigationMode == NavigationMode.New)
            {
                 SystemNavigationManager.GetForCurrentView().AppViewBackButtonVisibility = AppViewBackButtonVisibility.Visible;

                ViewModel = ((ViewModels.ItemViewModel)e.Parameter);

                if (ViewModel.SelectedItem == null)
                {
                    createButton.Content = "Create";
                }
                else
                {
                    createButton.Content = "Update";
                    Title.Text = ViewModel.SelectedItem.title;
                    Details.Text = ViewModel.SelectedItem.description;
                    Duedate.Date = ViewModel.SelectedItem.date;
                }

                ApplicationData.Current.LocalSettings.Values.Remove("TheWorkInProgress");
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
            if(createButton.Content.ToString() == "Update")
            {
                if (Title.Text == "" || Details.Text == "" || (Duedate.Date.ToUnixTimeSeconds()) + 86400 < (DateTimeOffset.Now.ToUnixTimeSeconds()))
                {
                    if (Title.Text == "")
                    {
                        var i = new MessageDialog("Title is empty").ShowAsync();
                    }

                    if (Details.Text == "")
                    {
                        var i = new MessageDialog("Details is empty").ShowAsync();
                    }

                    if ((Duedate.Date.ToUnixTimeSeconds()) + 86400 < (DateTimeOffset.Now.ToUnixTimeSeconds()))
                    {
                        var i = new MessageDialog("illegal date").ShowAsync();
                    }
                }

                else
                {
                    var db = App.conn;
                    var custstmt = db.Prepare("UPDATE TodoItem SET Title = ?, Context = ?, Date = ? WHERE Id = ?");
                             
                    custstmt.Bind(1, Title.Text);
                    custstmt.Bind(2, Details.Text);
                    custstmt.Bind(3, DateTimeOffset.Now.ToString());
                    custstmt.Bind(4, ViewModel.SelectedItem.id);
                    custstmt.Step();
                    

                    ViewModel.SelectedItem.title = Title.Text;
                    ViewModel.SelectedItem.description = Details.Text;
                    ViewModel.SelectedItem.date = Duedate.Date;
                    ViewModel.UpdateItem();
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
            }
                

            else
            {
                if (Title.Text == "" || Details.Text == "" || (Duedate.Date.ToUnixTimeSeconds()) + 86400 < (DateTimeOffset.Now.ToUnixTimeSeconds()))
                {
                    if (Title.Text == "")
                    {
                        var i = new MessageDialog("Title is empty").ShowAsync();
                    }

                    if (Details.Text == "")
                    {
                        var i = new MessageDialog("Details is empty").ShowAsync();
                    }

                    if ((Duedate.Date.ToUnixTimeSeconds()) + 86400 < (DateTimeOffset.Now.ToUnixTimeSeconds()))
                    {
                        var i = new MessageDialog("illegal date").ShowAsync();
                    }
                }

                else
                {
                    var db = App.conn;

                    var custstmt = db.Prepare("INSERT INTO TodoItem (Id, Title, Context, Date) VALUES (?, ?, ?, ?)");

                    string id = ViewModel.AddItem(Title.Text, Details.Text, Duedate.Date);

                    custstmt.Bind(1, id);
                    custstmt.Bind(2, Title.Text);
                    custstmt.Bind(3, Details.Text);
                    custstmt.Bind(4, DateTimeOffset.Now.ToString());
                    custstmt.Step();
                    Frame.Navigate(typeof(MainPage), ViewModel);
                }
            }
            
            }

        private void DeleteAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            var db = App.conn;
            var statement = db.Prepare("DELETE FROM TodoItem WHERE Id = ?");
            {
                statement.Bind(1, ViewModel.SelectedItem.id);
                statement.Step();
            }

            ViewModel.RemoveItem();
            Frame.Navigate(typeof(MainPage), ViewModel);
        }

    }
}
