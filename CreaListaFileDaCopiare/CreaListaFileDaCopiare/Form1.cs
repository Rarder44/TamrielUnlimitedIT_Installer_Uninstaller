using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CreaListaFileDaCopiare
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }


        private void button1_Click(object sender, EventArgs e)
        {
            if(openFileDialog1.ShowDialog()==DialogResult.OK)
            {
                textBox1.Text = Path.GetDirectoryName(openFileDialog1.FileName);
                string BasePath = textBox1.Text.Trim();

                if (BasePath == "")
                {
                    MessageBox.Show("Nessuna cartella settata");
                    Close();
                }

                if (!Directory.Exists(BasePath))
                {
                    MessageBox.Show("Cartella settata inesistente");
                    Close();
                }
                List<string> AllShortFolder = PathService.AbsoluteToRelative(FindAllFolder(BasePath), BasePath);
                List<string> AllFile = FindAllFile(BasePath);



                /*for (int i = 0; i < AllShortFolder.Count; i++)
                    NewGlobVar_cppCenter.Add("FolderToCreate.push_front(\"" + PathService.StripSlash(AllShortFolder[i]) + "\");");*/

                int c = 0;
                foreach (string s in AllFile)
                {
                    String t = PathService.StripSlash(PathService.AbsoluteToRelative(s, BasePath));
                    textBox2.Text += c+"= [TempFolder]\\" +  t+ "	[InstallFolder]\\" + t+"\r\n";
                    c++;
                }
            }
        }



        List<string> FindAllFolder(string Folder)
        {
            List<string> lf = new List<string>();
            foreach(string s in Directory.GetDirectories(Folder))
            {
                lf.Add(s);
                foreach (string ss in FindAllFolder(s))
                    lf.Add(ss);
            }
            return lf;

        }
        List<string> FindAllFile(string Folder)
        {
            List<string> lf = new List<string>();
            foreach (string s in Directory.GetFiles(Folder))
                lf.Add(s);
          
            foreach (string s in Directory.GetDirectories(Folder))
            {
                foreach (string ss in FindAllFile(s))
                    lf.Add(ss);
            }
            return lf;

        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.A)
                ((TextBox)sender).SelectAll();
        }
    }
}
