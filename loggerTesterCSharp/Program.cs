using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using loggerSrvLib;
namespace loggerTesterCSharp
{
	class Program
	{
		static void testForCorrectness()
		{
			Console.WriteLine("testing for correctness, press 'q' to quit.");
			Logger logger = new Logger();
			logger.Create("C:\\Users\\XPS\\cppProjects\\logger\\test_verifying.txt");
			bool quit = false;
			Random rnd = new Random();
			List<string> lstStandard = new List<string>();
			int n_t_0 = Environment.TickCount;
			int n_f = 0;
			int c_left = Console.CursorLeft;
			int c_top = Console.CursorTop;
			while (!quit)
			{
				int n = rnd.Next(2, 4096);
				string item = new string('b', n);
				item = '[' + item + ']';
				logger.LogOut(item);
				lstStandard.Add(item);
				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo keyInfo = Console.ReadKey(true);
					if (keyInfo.Key == ConsoleKey.Q)
						quit = true;
				}
				int n_t = Environment.TickCount - n_t_0;
				if (n_t > 1000)
				{
					float fps = ((float)n_f / (float)n_t) * 1000;
					Console.SetCursorPosition(c_left, c_top);
					Console.WriteLine(fps);
				}
				n_f++;
				System.Threading.Thread.Sleep(1);
			}
			logger.Close();

			FileStream fs = new FileStream("C:\\Users\\XPS\\cppProjects\\logger\\test_standard.txt"
										   , FileMode.CreateNew
										   , FileAccess.Write);
			BufferedStream buf = new BufferedStream(fs);
			foreach (string item in lstStandard)
			{
				byte[] bytes = Encoding.Unicode.GetBytes(item);
				buf.Write(bytes, 0, bytes.Length);
			}
			buf.Flush();
		}

		static void testForMemoryConsumption()
		{
			Console.WriteLine("testing for correctness, press 'q' to quit.");
			Logger logger = new Logger();
			logger.Create("C:\\Users\\XPS\\cppProjects\\logger\\test_verifying.txt");
			bool quit = false;
			Random rnd = new Random();
			int n_t_0 = Environment.TickCount;
			int n_f = 0;
			int c_left = Console.CursorLeft;
			int c_top = Console.CursorTop;
			while (!quit)
			{
				int n = rnd.Next(2, 4096);
				string item = new string('b', n);
				item = '[' + item + ']';
				logger.LogOut(item);
				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo keyInfo = Console.ReadKey(true);
					if (keyInfo.Key == ConsoleKey.Q)
						quit = true;
				}
				int n_t = Environment.TickCount - n_t_0;
				if (n_t > 1000)
				{
					float fps = ((float)n_f / (float)n_t) * 1000;
					Console.SetCursorPosition(c_left, c_top);
					Console.WriteLine(fps);
				}
				n_f++;
				System.Threading.Thread.Sleep(0);
			}
			logger.Close();


		}
		static void Main(string[] args)
		{

			Console.WriteLine("Start correctness or memory consumption test:");
			Console.WriteLine("\t1. for correctness test.");
			Console.WriteLine("\t2. for memory consumption test.");
			int command = Console.Read();
			if ('1' == command)
				testForCorrectness();
			else if (2 == command)
				testForMemoryConsumption();
		}
	}
}
