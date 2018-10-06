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

		deltaTime = time - t0;
		//Calcul and display the FPS
		if ((deltaTime) > 1.0 || frames == 0)
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
	float deltaTime;
};
