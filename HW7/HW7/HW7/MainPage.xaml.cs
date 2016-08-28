using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Xml;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Runtime.Serialization.Json;
using Newtonsoft.Json;
using Windows.Security.Cryptography;
using Windows.Security.Cryptography.Core;
using Windows.Storage.Streams;
using Windows.UI.Popups;

namespace HW7
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            string input_data = this.translate_word.Text;

            translate_and_print(input_data);
        }

        private async void translate_and_print(string word_for_translate)
        {
            HttpClient httpClient = new HttpClient();

            string url_for_translate_result = "http://sunzhongyang.com:8000/" + word_for_translate;

            HttpResponseMessage response = await httpClient.GetAsync(url_for_translate_result);
            response.EnsureSuccessStatusCode();

            Byte[] getByte = await response.Content.ReadAsByteArrayAsync();

            Encoding code = Encoding.GetEncoding("UTF-8");
            string result = code.GetString(getByte, 0, getByte.Length);

            JsonTextReader json = new JsonTextReader(new StringReader(result));
            string jsonVal = "", translate_result = "";

            while (json.Read())
            {
                jsonVal += json.Value;
                if (jsonVal.Equals("dst"))
                {
                    json.Read();
                    translate_result += json.Value;
                    break;
                }
                jsonVal = "";
            }

            Chinese_translate.Text = translate_result;
        }

        private async void Button_Click_1(object sender, RoutedEventArgs e)
        {
            string message = send_message.Text;
            string chat_id = name_Copy.Text;
            string name_spc = name.Text;
            string cond = "1";
            string time = DateTimeOffset.Now.DateTime.ToString();
            time = time.Replace(" ", "1kongge1");
            time = time.Replace(":", "1maohao1");
            time = time.Replace("/", "1xiegang1");
            message = message.Replace(" ", "1kongge1");

            string send = chat_id + "d4d" + name_spc + "d4d" + cond + "d4d" + time + "d4d" + message;

            HttpClient httpClient = new HttpClient();
            string url_for_translate_result = "http://sunzhongyang.com:8001/" + send;

            HttpResponseMessage response = await httpClient.GetAsync(url_for_translate_result);
            response.EnsureSuccessStatusCode();

            send_message.Text = "";
            string receive = await response.Content.ReadAsStringAsync();
            receive = receive.Replace("1kongge1", " ");
            receive = receive.Replace("1maohao1", ":");
            receive = receive.Replace("1xiegang1", "/");
            translate_word_Copy.Text = receive;
        }


        private async void Button_Click_2(object sender, RoutedEventArgs e)
        {
            string chat_id = name_Copy.Text;
            string send = chat_id + "d4d" + "update" + "d4d" + "0" + "d4d" + "2016" + "d4d" + "update";

            HttpClient httpClient = new HttpClient();
            string url_for_translate_result = "http://sunzhongyang.com:8001/" + send;

            HttpResponseMessage response = await httpClient.GetAsync(url_for_translate_result);
            response.EnsureSuccessStatusCode();

            send_message.Text = "";
            string receive = await response.Content.ReadAsStringAsync();
            receive = receive.Replace("1kongge1", " ");
            receive = receive.Replace("1maohao1", ":");
            receive = receive.Replace("1xiegang1", "/");
            translate_word_Copy.Text = receive;
        }
    }
}
