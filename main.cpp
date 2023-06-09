/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include <math.h>
#include <pspdebug.h>
#include <pspkernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUGL_IMPLEMENTATION
#include "imgui/imgui.h"

#include <GL/gl.h>   // Header File For The OpenGL32 Library
#include <GL/glu.h>  // Header File For The GLu32 Library
#include <GL/glut.h> // Header File For The GLUT Library

#include "imgui/backends/imgui_impl_glut.h"
#include "imgui/backends/imgui_impl_opengl2.h"

/* The number of our GLUT window */
int window;

/* white ambient light at half intensity (rgba) */
GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};

/* super bright, full intensity diffuse light. */
GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

/* position of light (x, y, z, (position of light)) */
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};



static int exitRequest = 0;

int running() { return !exitRequest; }
int exitCallback(int arg1, int arg2, void *common) {
  exitRequest = 1;
  return 0;
}
int callbackThread(SceSize args, void *argp) {
  int cbid;

  cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
  sceKernelRegisterExitCallback(cbid);

  sceKernelSleepThreadCB();

  return 0;
}
int setupCallbacks(void) {
  int thid = 0;

  thid =
      sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
  if (thid >= 0) {
    sceKernelStartThread(thid, 0, 0);
  }

  return thid;
}

PSP_MODULE_INFO("ImGUI Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);



/* A general OpenGL initialization function.  Sets all of the initial
 * parameters. */
GLvoid
InitGL(GLsizei Width,
       GLsizei Height) // We call this right after our OpenGL window is created.
{
  // LoadGLTextures();                           // load the textures.
  // glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.

  glClearColor(0.0f, 0.0f, 0.0f,
               0.0f);      // This Will Clear The Background Color To Black
  glClearDepth(1.0);       // Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);    // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST); // Enables Depth Testing
  glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // Reset The Projection Matrix

  gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f,
                 100.0f); // Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);

  // set up light number 1.
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);   // add lighting. (ambient)
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);   // add lighting. (diffuse).
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // set light position.
  glEnable(GL_LIGHT1);                              // turn light 1 on.
}


// Our state
static bool show_demo_window = true;
static bool show_another_window = true;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static int counter = 0;
static float f = 0.0f;

void MainLoopStep() {

  printf("MainLoopStep\n");

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGLUT_NewFrame();

  ImGuiIO &io = ImGui::GetIO();


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }


  // Rendering
  ImGui::Render();
  glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  // glUseProgram(0); // You may want this if using this code in an OpenGL 3+
  // context where shaders may be bound, but prefer using the GL3+ code.
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  setupCallbacks();
  /* Initialize GLUT state - glut will take any command line arguments that
     pertain to it or X Windows - look at its documentation at
     http://reality.sgi.com/mjk/spec3/spec3.html */
  glutInit(&argc, argv);

  /* Select type of Display mode:
   Double buffer
   RGBA color
   Alpha components supported
   Depth buffer */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  /* get a 640 x 480 window */
  glutInitWindowSize(480, 272); // @@@

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);

  /* Open a window */
  window = glutCreateWindow("Hello");

  /* Register the function to do all our OpenGL drawing. */
  /// 	functionnnnnnnnnnnnnnnnnnnn
  // glutDisplayFunc(&DrawGLScene);
  glutDisplayFunc(&MainLoopStep);

  /* Go fullscreen.  This is as soon as possible. */
  //    glutFullScreen(); @@@

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&MainLoopStep);

  /* Initialize our window. */
  InitGL(480, 272); // @@@

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NoMouse;


  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGui_ImplGLUT_Init();
  ImGui_ImplOpenGL2_Init();
  ImGui_ImplGLUT_InstallFuncs();

  glutMainLoop();

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGLUT_Shutdown();

  ImGui::DestroyContext();

  sceKernelExitGame();
  return 0;
}