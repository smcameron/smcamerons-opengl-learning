#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

/* cre1@762studios.com */

#include <GL/glfw.h>

int main(int argc, char *argv[])
{
	double currtime, firsttime, lasttime, dt, dtms, sleeptime;
	uint32_t frames = 0;

	printf("hello\n");

	if (glfwInit() != GL_TRUE) {
		printf("glfwInit failed\n");
		return 1;
	}

	/* 800x600, 8/8/8/8 of rgba, 24 bit z buffer, 8 bit stencil
	 * buffer, (z and stencil must add to 32).
	 */
	if (glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8,
				GLFW_WINDOW) != GL_TRUE) {
		printf("Can't create window\n");
		return 1;
	}

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	firsttime = glfwGetTime();
	lasttime = firsttime;

	while (1) {
		currtime = glfwGetTime();
		dt = currtime - lasttime;
		lasttime = currtime;
		dtms = dt * 1000.0;
		sleeptime = ((1.0 / 30.0) * 1000.0) - dtms;
		if (sleeptime > 0.0)
			usleep((useconds_t) (sleeptime * 1000.0));
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers();
		if (!glfwGetWindowParam(GLFW_OPENED))
			break;
		frames++;
	}

	printf("dt = %g, frames = %u, elapsed time = %g, f/s = %g\n", dt,
		frames, lasttime - firsttime,
			(double) frames / (lasttime - firsttime));

	glfwCloseWindow();
	glfwTerminate();
	return 0;
}

