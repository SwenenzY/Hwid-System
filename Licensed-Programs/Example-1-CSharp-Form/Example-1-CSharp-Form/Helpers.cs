using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json.Linq;

namespace Example_1_CSharp_Form
{
    static class Helpers
    {
        public static string GetCpuID()
        {
            var Mbs = new ManagementObjectSearcher("Select ProcessorId From Win32_processor"); // Search Processor
            ManagementObjectCollection MbsList = Mbs.Get(); // Get list
            foreach (ManagementObject Mo in MbsList) { return Mo["ProcessorId"].ToString(); } // if found ProcessorId return;
            return null;
        }
        public static string GetWebResponse(string URL)
        {
            HttpWebRequest Request = WebRequest.Create(URL) as HttpWebRequest; // Creating Request
            using (HttpWebResponse Response = Request.GetResponse() as HttpWebResponse) // Getting Response via web
            {
                StreamReader Reader = new StreamReader(Response.GetResponseStream()); // Reader for response
                return Reader.ReadToEnd(); // reading all data and return
            }
        }
        public static bool CheckLicense(string Data, string Key, string Element)
        // Data = Web Response
        // Key = Json segment
        // Element = CPU ID etc.
        {
            try
            {
                JObject MyParser = JObject.Parse(Data); // Parse json with  Newtonsoft.Json
                foreach (JToken ParsedElement in MyParser[Key]) // Searching ID
                {
                    if (ParsedElement.ToString() == Element)
                    { // if ID == ID
                        return true; // return true
                    }
                }
                return false; // if cannot parse error etc.
            }
            catch (Exception)
            {
                return false; // if cannot parse error etc.
            }
        }
    }
}
