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
#include <GL/gl.h>

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

GLuint create_shader_program(void)
{
	GLuint proghandle;
	GLuint vshaderhandle;
	GLuint fshaderhandle;
	GLint length, result;
	char *vprog, *fprog, *logbuff;

	vprog = read_file("basic.v.glsl");
	printf("vprog = '%s'\n", vprog);
	fprog = read_file("basic.f.glsl");

	if (!vprog || !fprog) {
		fprintf(stderr, "Failed to read shader programs\n");
		free(vprog);
		free(fprog);
		return 0;
	}

	vshaderhandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshaderhandle, 1, (const GLchar **) &vprog, NULL);
		/* handle, number of strings, program(s), NULL ); */
	free(vprog);
	glCompileShader(vshaderhandle);
	glGetShaderiv(vshaderhandle, GL_COMPILE_STATUS, &result);
	if (result == 0) {
		glGetShaderiv(vshaderhandle, GL_INFO_LOG_LENGTH, &length);
		logbuff = calloc(length + 1, sizeof(char));
		glGetShaderInfoLog(vshaderhandle, length, NULL, logbuff);
		printf("Unable to compile vertex shader:\n%s\n", logbuff);
		free(logbuff);
		glDeleteShader(vshaderhandle);
		return 0;
	}

	fshaderhandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshaderhandle, 1, (const GLchar **) &fprog, NULL);
	free(fprog);
	glCompileShader(fshaderhandle);
	glGetShaderiv(fshaderhandle, GL_COMPILE_STATUS, &result);
	if (result == 0) {
		glGetShaderiv(fshaderhandle, GL_INFO_LOG_LENGTH, &length);
		logbuff = calloc(length + 1, 1);
		glGetShaderInfoLog(fshaderhandle, length, NULL, logbuff);
		printf("Unable to compile fragment shader:\n%s\n",
			logbuff);
		free(logbuff);
		glDeleteShader(vshaderhandle);
		glDeleteShader(fshaderhandle);
		return 0;
	}

	proghandle = glCreateProgram();
	glAttachShader(proghandle, vshaderhandle);
	glAttachShader(proghandle, fshaderhandle);
	glLinkProgram(proghandle);
	glGetProgramiv(proghandle, GL_LINK_STATUS, &result);
	if (result == 0) {
		glGetProgramiv(proghandle, GL_INFO_LOG_LENGTH, &length);
		logbuff = calloc(length + 1, 1);
		glGetProgramInfoLog(proghandle, length, NULL, logbuff);
		printf("Unable to link shader program:\n%s\n",
			logbuff);
		free(logbuff);
		glDeleteShader(vshaderhandle);
		glDeleteShader(fshaderhandle);
		glDeleteProgram(proghandle);
		return 0;
	}
	return proghandle;
}

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

	create_shader_program();

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

