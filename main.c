#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

/* Chris's email: cre1@762studios.com */

#include <GL/glfw.h>

char *read_file(char *f)
{
	int fd, rc;
	struct stat statbuf;
	char *buffer;

	fd = open(f, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open: %s\n", strerror(errno));
		return NULL;
	}
	rc = fstat(fd, &statbuf);
	if (rc < 0) {
		fprintf(stderr, "fstat: %s\n", strerror(errno));
		return NULL;
	}

	buffer = malloc(statbuf.st_size + 1);
	buffer[statbuf.st_size] = '\0';

	rc = read(fd, buffer, statbuf.st_size);
	close(fd);
	if (rc != statbuf.st_size) {
		fprintf(stderr, "Failed to read file\n"); 
		free(buffer);
		return NULL;
	}
	return buffer;
}

int main(int argc, char *argv[])
{
	double currtime, firsttime, lasttime, dt, dtms, sleeptime;
	uint32_t frames = 0;
	char *vertex_shader_source;
	char *fragment_shader_source;

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

	vertex_shader_source = read_file("basic.v.glsl");
	fragment_shader_source = read_file("basic.f.glsl");

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

	printf("vertex shader source = %s\n", 
		vertex_shader_source);
	printf("fragment shader source = %s\n", 
		fragment_shader_source);

	glfwCloseWindow();
	glfwTerminate();
	return 0;
}

