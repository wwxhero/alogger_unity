using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
				System.Threading.Thread.Sleep(1);
			}
			logger.Close();
		}

		//definition for Hiccups:
		//	1. fps < 60
		//	2. d(fps)/dn_f > epsilon where epsilon is a predefined value
		//  3. (Sigma |dfps_1|)/n_f1 > (Sigma |dfps_2|)/n_f2 -> fps_2 is smoother than fps_1
		static void testForHiccups(bool logging)
		{
			Console.WriteLine("testing for correctness, press 'q' to quit.");
			Logger logger = new Logger();
			logger.Create("C:\\Users\\XPS\\cppProjects\\logger\\test_verifying.txt");
			bool quit = false;
			Random rnd = new Random();
			int n_t_0 = Environment.TickCount;
			int n_f = 0;
			int n_t = 0;
			int c_left = Console.CursorLeft;
			int c_top = Console.CursorTop;
			//skip the starting a second
			while (!quit && n_t < 1000)
			{
				string item = new string('b', rnd.Next(2, 4096));
				item = '[' + item + ']';
				if (logging)
					logger.LogOut(item);
				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo keyInfo = Console.ReadKey(true);
					if (keyInfo.Key == ConsoleKey.Q)
						quit = true;
				}
				n_t = Environment.TickCount - n_t_0;
				if (n_t > 1000)
				{
					float fps = ((float)n_f / (float)n_t) * 1000;
					Console.SetCursorPosition(c_left, c_top);
					Console.WriteLine(fps);
				}
				n_f++;
				System.Threading.Thread.Sleep(1);
			}

			float fps_im = ((float)n_f / (float)n_t) * 1000;
			bool hiccups = false;
			int n = 0;
			float epsilon = 0.05f;
			float sigma_dfps = 0;
			float max_dfps = 0;
			float min_dfps = float.MaxValue;
			while (!quit && !hiccups)
			{
				string item = new string('b', rnd.Next(2, 4096));
				item = '[' + item + ']';
				if (logging)
					logger.LogOut(item);
				if (Console.KeyAvailable)
				{
					ConsoleKeyInfo keyInfo = Console.ReadKey(true);
					if (keyInfo.Key == ConsoleKey.Q)
						quit = true;
				}
				n_t = Environment.TickCount - n_t_0;

				float fps = ((float)n_f / (float)n_t) * 1000;
				Console.SetCursorPosition(c_left, c_top);
				Console.WriteLine(fps);

				float dfps = Math.Abs(fps - fps_im);
                fps_im = fps;
				hiccups = (fps < 60
						|| dfps > epsilon * fps);
				sigma_dfps += dfps;
				if (min_dfps > dfps)
					min_dfps = dfps;
				if (max_dfps < dfps)
					max_dfps = dfps;

				n_f ++;
				n ++;


                System.Threading.Thread.Sleep(1);
			}
			logger.Close();

			if (hiccups)
				Console.WriteLine("hiccups happens!!!");
			else
			{
				float score = sigma_dfps/(float)n;
                string strInfo = string.Format("hiccups does not happen, smooth score: {0}, min dfps: {1}, max dfps: {2}"
                							, score
                							, min_dfps
                							, max_dfps);
				Console.WriteLine(strInfo);
			}
		}

		static void Main(string[] args)
		{

			Console.WriteLine("Start correctness or memory consumption test:");
			Console.WriteLine("\t1. for correctness test.");
			Console.WriteLine("\t2. for memory consumption test.");
			Console.WriteLine("\t3. for hiccups test with logging.");
			Console.WriteLine("\t4. for hiccups test without logging.");
			int command = Console.Read();
			if ('1' == command)
				testForCorrectness();
			else if ('2' == command)
				testForMemoryConsumption();
			else if ('3' == command)
				testForHiccups(true);
			else if ('4' == command)
				testForHiccups(false);
		}
	}
}
