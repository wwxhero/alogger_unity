using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;
public class loggerTesterCorrecthicup : MonoBehaviour {
	loggerSrvLib.Logger m_logger;
	Random m_rnd = new Random();
	float m_t;
	float m_t0;
	int m_f = 0;
	// Use this for initialization
	void Start () {
		m_logger = new loggerSrvLib.Logger();
		m_logger.Create("test_verifying.txt");
		m_t0 = Environment.TickCount;
	}

	// Update is called once per frame
	void Update () {
		int n = Random.Range(2, 4096);
		string item = new string('b', n);
				item = '[' + item + ']';
		m_t = Environment.TickCount - m_t0;
		if (m_t > 1000)
		{
			float fps = ((float)m_f / (float)m_t) * 1000;
			int fps_i = Math.bottom(fps);
			string fps_s = string.Format("fps={0, 3:D}", fps_i);
			item += fps_s
		}
		m_f ++;
		m_logger.LogOut(item);
	}

    void OnDestroy()  {
    	m_logger.Close();
    	m_logger = null;
    }
}
