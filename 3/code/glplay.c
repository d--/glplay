#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <GL/glew.h>
#endif
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

/******************************* TUTORIAL CODE *******************************/

GLuint CreateShader(GLenum eShaderType, const char * shaderStr) {
    GLuint shader = glCreateShader(eShaderType);

    glShaderSource(shader, 1, &shaderStr, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = 
            (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch (eShaderType) {
            case GL_VERTEX_SHADER:
                strShaderType = "vertex";
                break;
            case GL_GEOMETRY_SHADER:
                strShaderType = "geometry";
                break;
            case GL_FRAGMENT_SHADER:
                strShaderType = "fragment";
                break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n",
                strShaderType, strInfoLog);
        free(strInfoLog);
    }

    return shader;
}

GLuint CreateProgram(const GLuint * shaders, size_t len) {
    GLuint program = glCreateProgram();

    for (size_t i = 0; i < len; i++) {
        glAttachShader(program, shaders[i]);
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = 
            (GLchar *)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        free(strInfoLog);
    }

    for (size_t i = 0; i < len; i++) {
        glDetachShader(program, shaders[i]);
    }

    return program;
}

const char * strVertexShader = (
    "#version 330\n"
    "layout(location = 0) in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = position;\n"
    "}\n"
);

const char * strFragmentShader = (
    "#version 330\n"
    "out vec4 outputColor;\n"
    "void main()\n"
    "{\n"
    "   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n"
);

GLuint program;

void initProgram() {
    GLuint shaders[2];
    shaders[0] = CreateShader(GL_VERTEX_SHADER, strVertexShader);
    shaders[1] = CreateShader(GL_FRAGMENT_SHADER, strFragmentShader);
    program = CreateProgram(shaders, 2);
}

const float vertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f    
};

GLuint posBufObj;

void initVertexBuffer() {
    glGenBuffers(1, &posBufObj);

    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions),
    vertexPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void init() {
    initProgram();
    initVertexBuffer();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

/*************************** END TUTORIAL CODE *******************************/
void key_handler(GLFWwindow* win, int key, int scancode,
        int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GL_TRUE);
}

int w_width = 640;
int w_height = 480;
float aspect_ratio = (640.0f / 480.0f);

void resize_handler(GLFWwindow* win, int width, int height) {
    float v_width = (float) width;
    float v_height = (float) height;

    float h;
    if (height > (h = ((float) width / aspect_ratio))) {
        v_height = h;
    }
    float w;
    if (width > (w = ((float) height * aspect_ratio))) {
        v_width = w;
    }

    float x = (width - v_width) / 2.f;
    float y = (height - v_height) / 2.f;

    glViewport(x, y, v_width, v_height);
    render();
    glfwSwapBuffers(win);
}

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(w_width, w_height, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

#ifdef _WIN32
    /* Initialize extensions */
    GLenum err = glewInit();
    if (err != GLEW_OK)
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
#endif

    /* Register input callback */
    glfwSetKeyCallback(window, key_handler);

    /* Register window size (framebuffer size) callback */
    glfwSetFramebufferSizeCallback(window, resize_handler);

    init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
