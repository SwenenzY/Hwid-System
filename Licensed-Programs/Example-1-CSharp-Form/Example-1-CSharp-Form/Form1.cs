using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Example_1_CSharp_Form
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            string WebResponseData = Helpers.GetWebResponse("http://127.0.0.1/list"); // Getting Response
            string CpuID = Helpers.GetCpuID(); // Getting CPU ID
            bool LicenseStatus = Helpers.CheckLicense(WebResponseData, "Keys", CpuID); // Checking License
            label1.Text = LicenseStatus ? "Have a license" : "Sorry :(";// ? : instead  of if else (Basicly)
        }
    }
}
