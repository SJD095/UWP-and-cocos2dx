using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW2.Models
{
    class Item
    {

        private string id;

        public string title { get; set; }

        public string description { get; set; }

        public string completed { get; set; }

        public DateTimeOffset date { get; set; }

    public Item(string title, string description, DateTimeOffset date)
        {
            this.date = date;
            this.id = Guid.NewGuid().ToString();
            this.title = title;
            this.description = description;
            this.completed = "None";
        }
    }
}
