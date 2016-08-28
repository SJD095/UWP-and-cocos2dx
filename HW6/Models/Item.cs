using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW2.Models
{
    class Item
    {

        public string id;

        public string title { get; set; }

        public string description { get; set; }

        public string completed { get; set; }

        public DateTimeOffset date { get; set; }

    public Item(string id, string title, string description, DateTimeOffset date)
        {
            this.date = date;
            this.id = id;
            this.title = title;
            this.description = description;
            this.completed = "None";
        }
    }
}
