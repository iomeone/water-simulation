#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "util/util.h"
#include "maths/maths.h"
#include "objects/rectangle.h"
#include "objects/water.h"
#include "shaders/shader.h"

int main() {
    GLFWwindow *window = util_init_gl_window("opengl", 1000, 1000);

    vec3 cam_pos(5, 8, 10); 
    vec3 cam_targ(0, 0, 0);
    vec3 cam_up(0, 1, 0);
    mat4 view_mat = mat4::look_at(cam_pos, cam_targ, cam_up);
    mat4 proj_mat = mat4::perspective_projection(60, 1, 0.001, 20);

    graphics::shader shader("shaders/default.vert", "shaders/default.frag");
    object3d::rectangle rectangle(vec3(0, 0, 0), vec3(0, 1, 0));

    object3d::water water;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the rectangles
        glUseProgram(shader.program);

        water.update(1/60.0);

        for (int i = 0; i < water.width; i++) {
            for (int j = 0; j < water.height; j++) {
                float x = -3 + 6 * (1 - (i / (float) water.width));
                float y = -3 + 6 * (1 - (j / (float) water.height));

                vec3 position(x, 0, y);
                vec3 scale(1.0, water.u[i][j], 1.0);

                mat4 model_mat = mat4::translation(position) * mat4::scale(scale);
                    
                glUniformMatrix4fv(shader.model_mat_location, 1, GL_TRUE, model_mat.m);
                glUniformMatrix4fv(shader.view_mat_location, 1, GL_TRUE, view_mat.m);
                glUniformMatrix4fv(shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);

                glBindVertexArray(rectangle.vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
