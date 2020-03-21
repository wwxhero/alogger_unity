using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using loggerSrvLib;
namespace loggerTesterCSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            Logger logger = new Logger();
            logger.Create("C:\\Users\\XPS\\cppProjects\\logger\\x64\\Debug\\test.txt");
            logger.Close();
        }
    }
}
