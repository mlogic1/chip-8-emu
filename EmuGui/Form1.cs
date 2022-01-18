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

namespace EmuGui
{
	public partial class Form1 : Form
	{
		Bitmap mBitmap;

		public Form1()
		{
			InitializeComponent();
			// mBitmap = new Bitmap(64, 32);
			// b.SetPixel(20, 20, Color.Black);
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			using (Stream BitmapStream = System.IO.File.Open("C:\\Users\\Filip\\Desktop\\asd.jpg", System.IO.FileMode.Open))
			{
				Image img = Image.FromStream(BitmapStream);

				mBitmap = new Bitmap(img);
				pictureBoxRenderer.Image = mBitmap;
			}

			// pictureBoxRenderer.Image = mBitmap;
		}

		private void loadROMToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog open = new OpenFileDialog();
			open.Title = "Select CHIP-8 ROM file";
			if (open.ShowDialog() == DialogResult.OK)
			{
				string file = open.FileName;
				// load rom
			}
		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show("CHIP-8 interperter.\nAuthor: mlogic1.\nClick File->Load ROM and enjoy.", "About", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Quit();
		}

		private void Quit()
		{
			// free any reserved data
			Application.Exit();
		}

		
	}
}
