using System;

namespace Example_1_CSharp_Console
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Title = "Open Source ❤️ SwenenzY";
            string WebResponseData = Helpers.GetWebResponse("http://127.0.0.1/list"); // Getting Response
            string CpuID = Helpers.GetCpuID(); // Getting CPU ID
            bool LicenseStatus = Helpers.CheckLicense(WebResponseData, "Keys", CpuID); // Checking License
            Console.WriteLine(LicenseStatus ? "Have a license" : "Sorry :(" ); // ? : instead  of if else (Basicly)
            Console.Read();
        }
    }
}
