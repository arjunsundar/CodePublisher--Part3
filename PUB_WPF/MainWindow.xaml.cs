/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs : WPF Client that uses Shim to call native C++ code  //
// ver 1.0                                                                 //
//                                                                         //
// Application    : CSE-687 C++/CLI Shim                                   //
// Platform       : Visual Studio 2017 Community Edition                   //
//                  HP intel core i7                                       //
// Author         : Arjun Kalyanasundaram, Syracuse University             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/*
 *  Package Decription:
 * =====================
 *  This package defines the GUI using MainWindow.xaml file which is compiled
 *  to a working GUI saving you the effort of programatically creating GUIs.
 *  The package communicates with the UI made in MainWindow.xaml. A number of 
 *  features have been created in this GUI. A navigation bar, a converted files 
 *  folder. The publish button converts all the source code using the neccessary Regex
 *  and converts then into webpages.
 *  Public Interface: N/A
 *  
 *  Required Files:
 * =================
 *  MainWindow.xaml MainWindow.xaml.cs Shim.dll
 *  
 *  Build Process:
 * ================
 *  msbuild PUB_WPF.csproj
 *  
 *  Maintainence History:
 * =======================
 *  ver 1.0 - April 6th, 2019
 *    - first release
 */


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.IO;

namespace Pub_WPF
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window              //created a class for the main window
    {
        Shim shim;                                       //An instance of the shim is being created
     
        private string WDirectory { get; set; }  
        private string Patterns { get; set; }
        private string Regex { get; set; }

        public MainWindow()
        {
            InitializeComponent();
        }

        //------<function consists of fixed input items>-----//
        private void Window_Loaded(object sender, RoutedEventArgs e)     
        {
            WDirectory = "../../../";
            WDirectory = Path.GetFullPath(WDirectory);
            Patterns = "*.h *.cpp"; //setting the inputs for Patterns
            Regex = "[A-B](.*)";    // setting the inputs for Regex
            txtpath.Text = WDirectory;
            txtPatterns.Text = Patterns;   
            txtRegex.Text = Regex;
            btnPublish_Click(sender, e);  ///publish button in loaded window to publish when program is run
        }

       
        
        //------<BROWSE button to browse files through directories>-----//
        private void btnBrowse_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dialog;
            dialog = new System.Windows.Forms.FolderBrowserDialog();
            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                WDirectory = dialog.SelectedPath;
                txtpath.Text = WDirectory;
            }
        }
        //------<Publish button to publish the converted files>-----//

        private void btnPublish_Click(object sender, RoutedEventArgs e)
        {
            List<string> args = new List<string>();  
            args.Add("PUB_WPF.exe");
            args.Add(WDirectory);
            if (cbRecurse.IsChecked.HasValue)    //Recusring through the values
            args.Add("/s");
            args.AddRange(Patterns.Split(' '));
            args.AddRange(Regex.Split(' '));
            shim = new Shim();
            List<string> final_list = new List<string>();
            final_list = shim.SendCmd(args);      // obtaining a list of converted files after arguments has been sent
            foreach (var v in final_list)
            {
                listConverted.Items.Add(v);                 
            }
            int i = 0;
                foreach (var file in final_list)
                {
                if (i < 2)               // displays a maximum of 2 converted files when published
                {
                    System.Diagnostics.Process x = new System.Diagnostics.Process();
                    x.StartInfo.FileName = file;
                    x.Start();     //displaying the converted files
                    x.WaitForExit();
                }
                i = i + 1;
                }
        }

        //------<Adding files to a list and able to view selected files using Regex>-----//
        private void txtpath_TextChanged(object sender, TextChangedEventArgs e)
        {
            lstfiles.Items.Clear();         
            lstfiles.Items.Add("[..]");
            var dirs = Directory.GetDirectories(WDirectory);
            foreach (var dir in dirs)
            {
                string dirName = "[" + Path.GetFileName(dir) + "]";

                lstfiles.Items.Add(dirName);

            }
            List<string> filesMatchingPatterns = new List<string>();
            if (Patterns != null)
            {
                var patterns = Patterns.Split(' ');            //matching the files using the given patterns

                foreach (var patt in patterns)
                    filesMatchingPatterns.AddRange(Directory.GetFiles(WDirectory, patt));    
            }
            else
            {
                filesMatchingPatterns = Directory.GetFiles(WDirectory).ToList();
            }
            foreach (var file in filesMatchingPatterns) lstfiles.Items.Add(Path.GetFileName(file));   //adding the files with a matched pattern into a list 

        }

        //------<Double Click option to view additional files in the list>-----//
        private void lstfiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

            if (lstfiles.SelectedIndex == -1) return;
            string selectedItem = lstfiles.SelectedItem.ToString();
            selectedItem = selectedItem.Substring(1, selectedItem.Length - 2);
            selectedItem = Path.Combine(WDirectory, selectedItem);
            selectedItem = Path.GetFullPath(selectedItem);               //returns the full path of the selected item
            if (!Directory.Exists(selectedItem)) return;
            WDirectory = selectedItem;
            txtpath.Text = selectedItem;
        }

        //------<Double Click option to view the converted files in a webpage>-----//
        private void listConverted_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string converted = listConverted.SelectedItem.ToString();
            System.Diagnostics.Process.Start(converted);               //opens a webpage when double clicked on the converted file
        }
        //------<Input for Patterns through the box>-----//
        private void txtPatterns_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Patterns = txtPatterns.Text;
                txtpath_TextChanged(this, null);

            }
        }
        //------<Input for Regex through the box>-----//
        private void txtRegex_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Regex = txtRegex.Text;
                txtpath_TextChanged(this, null);

            }
        }
    }
}

