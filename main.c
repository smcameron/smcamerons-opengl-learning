#include <stdio.h>
#include <unistd.h>

/* cre1@762studios.com */

#include <GL/glfw.h>

int main(int argc, char *argv[])
{
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
	sleep(5);
	glfwCloseWindow();
	glfwTerminate();
	return 0;
}

