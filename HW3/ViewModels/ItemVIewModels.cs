using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW2.ViewModels
{
    class ItemViewModel
    {
        private ObservableCollection<Models.Item> allItems = new ObservableCollection<Models.Item>();
        public ObservableCollection<Models.Item> AllItems { get { return this.allItems; } }

        private Models.Item selectedItem = default(Models.Item);
        public Models.Item SelectedItem { get { return selectedItem; } set { this.selectedItem = value; } }

        public ItemViewModel()
        {
            this.allItems.Add(new Models.Item("abc", "123", DateTimeOffset.Now));
        }

        public void AddItem(string title, string description, DateTimeOffset date)
        {
            this.allItems.Add(new Models.Item(title, description, date));
        }

        public void RemoveItem()
        {
            this.allItems.Remove(this.selectedItem);
            this.selectedItem = null;
        }

        public void UpdateItem()
        {
            this.selectedItem = null;
        }

    }
}
