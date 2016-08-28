using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace Animal
{
    public sealed partial class MainPage : Page
    {
        private delegate string AnimalSaying(object sender, myEventArgs e);
        private event AnimalSaying Say;
        private int times = 0;

        public MainPage()
        {
            this.InitializeComponent();
        }

        interface Animal
        {

            string saying(object sender, myEventArgs e);

            int A { get; set; }
        }

        class cat : Animal
        {
            TextBlock word;
            private int a;

            public cat(TextBlock w)
            {
                this.word = w;
            }
            public string saying(object sender, myEventArgs e)
            {
                this.word.Text += "I am a Cat" + "\n";
                return "";
            }

            public string sayings(object sender, myEventArgs e)
            {
                this.word.Text += "Cat: I am a Cat\n";
                return "";
            }
            public int A
            {
                get { return a; }
                set { this.a = value; }
            }
        }

        class dog : Animal
        {
            TextBlock word;
            private int a;

            public dog(TextBlock w)
            {
                this.word = w;
            }
            public string saying(object sender, myEventArgs e)
            {
                this.word.Text +="I am a Dog\n";
                return "";
            }

            public string sayings(object sender, myEventArgs e)
            {
                this.word.Text += "Dog: I am a Dog\n";
                return "";
            }
            public int A
            {
                get { return a; }
                set { this.a = value; }
            }
        }

        class pig : Animal
        {
            TextBlock word;
            private int a;

            public pig(TextBlock w)
            {
                this.word = w;
            }
            public string saying(object sender, myEventArgs e)
            {
                this.word.Text +="I am a Pig\n";
                return "";
            }

            public string sayings(object sender, myEventArgs e)
            {
                this.word.Text += "Pig: I am a Pig\n";
                return "";
            }

            public int A
            {
                get { return a; }
                set { this.a = value; }
            }
        }

        private cat c;
        private dog d;
        private pig p;

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Random ran = new Random();
            int r_number;
            r_number = ran.Next();

                words.Text = "";
                c = new cat(words);
                d = new dog(words);
                p = new pig(words);
               
                switch(r_number%3)
                {
                case 0:
                    Say += new AnimalSaying(c.saying);
                    break;
                case 1:
                    Say += new AnimalSaying(d.saying);
                    break;
                case 2:
                    Say += new AnimalSaying(p.saying);
                    break;
        }
            Say(this, new myEventArgs(times++));
        }

        class myEventArgs : EventArgs
        {
            public int t = 0;
            public myEventArgs(int tt)
            {
                this.t = tt;
            }
        }

        private void configure_Click(object sender, RoutedEventArgs e)
        {
            string str = textBox.Text.ToString();
            words.Text = "";
            c = new cat(words);
            d = new dog(words);
            p = new pig(words);
            if (str != "")
            {
                switch (str)
                {
                    case "cat":
                        
                        Say += new AnimalSaying(c.sayings);
                        break;
                    case "dog":
                        Say += new AnimalSaying(d.sayings);
                        break;
                    case "pig":
                        Say += new AnimalSaying(p.sayings);
                        break;
                }
            }
            Say(this, new myEventArgs(times++));
        }
    }
}
