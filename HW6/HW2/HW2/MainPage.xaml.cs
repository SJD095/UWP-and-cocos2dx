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
using Windows.UI.Xaml.Shapes;
using Windows.UI.Notifications;
using Windows.UI.StartScreen;
using Windows.Data.Xml.Dom;
using NotificationsExtensions.Tiles;
using Windows.ApplicationModel.DataTransfer;
using SQLitePCL;

namespace HW2
{
    public sealed partial class MainPage : Page
    {

        ViewModels.ItemViewModel ViewModel { get; set; }
        DataTransferManager dtm;

        public MainPage()
        {
            this.InitializeComponent();
            var viewTitleBar = Windows.UI.ViewManagement.ApplicationView.GetForCurrentView().TitleBar;
            viewTitleBar.BackgroundColor = Windows.UI.Colors.CornflowerBlue;
            viewTitleBar.ButtonBackgroundColor = Windows.UI.Colors.CornflowerBlue;
            this.ViewModel = new ViewModels.ItemViewModel();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            dtm = DataTransferManager.GetForCurrentView();
            dtm.DataRequested += dtm_DataRequested;

            if (e.Parameter.GetType() == typeof(ViewModels.ItemViewModel))
            {
                this.ViewModel = (ViewModels.ItemViewModel)(e.Parameter);
            }
        }

        private void AddAppBarButton_Click(object sender, RoutedEventArgs e)
        {
            //if (this.ActualWidth > 1000)
            //{
            //    string a = Convert.ToString(this.ActualWidth);
            //    var i = new MessageDialog(a).ShowAsync();
            //    return;
            //}
            //else 
            Frame.Navigate(typeof(NewPage), ViewModel);
        }

        private void TodoItem_ItemClicked(object sender, ItemClickEventArgs e)
        {
            
            ViewModel.SelectedItem = (Models.Item)(e.ClickedItem);
            Frame.Navigate(typeof(NewPage), ViewModel);
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            if (Title.Text != "")
            {
                Title.Text = "";
            }

            if (Details.Text != "")
            {
                Details.Text = "";
            }

            Duedate.Date = DateTimeOffset.Now;
        }

        private void createButton_Click(object sender, RoutedEventArgs e)
        {
            if (createButton.Content.ToString() == "Update")
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

        private void CheckBox1_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox mn = (CheckBox)e.OriginalSource;
            ViewModel.SelectedItem = (Models.Item)(mn.DataContext);

            ViewModel.SelectedItem.completed = "Fill";
        }

        private void AddAppBarButton_Click_1(object sender, RoutedEventArgs e)
        {
            Frame.Navigate(typeof(Page2));
        }


        private async void button1_Click(object sender, RoutedEventArgs e)
        {
            string from = "title";
            string subject = ViewModel.AllItems[0].title;

            TileContent content = new TileContent()
            {
                Visual = new TileVisual()
                {
                    DisplayName = "Todos",

                    TileSmall = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new TileText()
                                {
                                    Text = from
                                },

                                new TileText()
                                {
                                    Text = subject,
                                    Style = TileTextStyle.CaptionSubtle
                                },
                            }
                        }
                    },

                    TileMedium = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new TileText()
                                {
                                    Text = from
                                },

                                new TileText()
                                {
                                    Text = subject,
                                    Style = TileTextStyle.CaptionSubtle
                                },
                            }
                        }
                    },

                    TileWide = new TileBinding()
                    {
                        Content = new TileBindingContentAdaptive()
                        {
                            Children =
                            {
                                new TileText()
                                {
                                    Text = from,
                                    Style = TileTextStyle.Subtitle
                                },

                                new TileText()
                                {
                                    Text = subject,
                                    Style = TileTextStyle.CaptionSubtle
                                },
                            }
                        }
                    }
                }
            };

            var notification = new TileNotification(content.GetXml());

            TileUpdateManager.CreateTileUpdaterForApplication().Update(notification);

            if (SecondaryTile.Exists("MySecondaryTile"))
            {
                var updater = TileUpdateManager.CreateTileUpdaterForSecondaryTile("MySecondaryTile");

                updater.Update(notification);
            }
        }


        private void MenuFlyoutItem_Click(object sender, RoutedEventArgs e)
        {
            MenuFlyoutItem mn = (MenuFlyoutItem)e.OriginalSource;
            ViewModel.SelectedItem = (Models.Item)(mn.DataContext);
            DataTransferManager.ShowShareUI();
        }

        void dtm_DataRequested(DataTransferManager sender, DataRequestedEventArgs e)
        {
            string textSource = ViewModel.SelectedItem.description;
            string textTitle = "Title";
            DataPackage data = e.Request.Data;
            data.Properties.Title = textTitle;
            data.SetText(textSource);
        }

        public void Button_Click(object sender, RoutedEventArgs e)
        {
            string query = Query.Text;
            query = "%" + query + "%";

            TodoItem todo = Sql_Select(query);

            var i = new MessageDialog(todo.result).ShowAsync();
        }

        public class TodoItem
        {
            public string result;
        }

        public TodoItem Sql_Select(string query_input)
        {
            TodoItem todo_tmp = new TodoItem();

            var db = App.conn;
            var statement = db.Prepare("SELECT Id, Title, Context, Date FROM TodoItem WHERE Title LIKE ? or Id LIKE ? or Context LIKE ? or Date LIKE ?");
            statement.Bind(1, query_input);
            statement.Bind(2, query_input);
            statement.Bind(3, query_input);
            statement.Bind(4, query_input);

            do
            {
                statement.Step();
                if (statement[0] == null) break;
                todo_tmp.result += "Id: " + (string)statement[0];
                todo_tmp.result += " Title: " + (string)statement[1];
                todo_tmp.result += " Context: " + (string)statement[2];
                todo_tmp.result += " Date: " + (string)statement[3];
                todo_tmp.result += "\n";
            } while (true);
            
            return todo_tmp;
        }

        private void Edit_Click(object sender, RoutedEventArgs e)
        {
            MenuFlyoutItem mn = (MenuFlyoutItem)e.OriginalSource;
            ViewModel.SelectedItem = (Models.Item)(mn.DataContext);

            Frame.Navigate(typeof(NewPage), ViewModel);

        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            MenuFlyoutItem mn = (MenuFlyoutItem)e.OriginalSource;
            ViewModel.SelectedItem = (Models.Item)(mn.DataContext);

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