#pragma once



class MiscUtils
{
public:
	MiscUtils()
	{
		frames = 0;
		t0 = glfwGetTime();
	}

	void frameCount()
	{
		//get the current time
		time = glfwGetTime();

		//Calcul and display the FPS
		if ((time - t0) > 1.0 || frames == 0)
		{
			fps = (double)frames / (time - t0);
			std::cout << fps << std::endl;
			t0 = time;
			frames = 0;
		}
		frames++;
	}

private:
	int    frames;
	double fps, time;
	double t0;


};
//
//
//#pragma once
//
//
//#include <iostream>
//
//
//
//
//class MiscUtils
//{
//public:
//	MiscUtils()
//	{
//	}
//
//	void frameCount()
//	{
//
//		std::cout << "time "<< t0 << std::endl;
//		
//	}
//
//private:
//	int    frames = 3;
//	double fps, time;
//	double t0 = glfwGetTime();
//
//
//};
//
//
