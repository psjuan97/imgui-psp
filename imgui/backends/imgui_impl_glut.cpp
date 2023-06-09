// dear imgui: Platform Backend for GLUT/FreeGLUT
// This needs to be used along with a Renderer (e.g. OpenGL2)

// !!! GLUT/FreeGLUT IS OBSOLETE PREHISTORIC SOFTWARE. Using GLUT is not
// recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT today, you are being abused.
// Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

// Implemented features:
//  [X] Platform: Partial keyboard support. Since 1.87 we are using the
//  io.AddKeyEvent() function. Pass ImGuiKey values to all key functions e.g.
//  ImGui::IsKeyPressed(ImGuiKey_Space). [Legacy GLUT values will also be
//  supported unless IMGUI_DISABLE_OBSOLETE_KEYIO is set]
// Issues:
//  [ ] Platform: GLUT is unable to distinguish e.g. Backspace from CTRL+H or
//  TAB from CTRL+I [ ] Platform: Missing horizontal mouse wheel support. [ ]
//  Platform: Missing mouse cursor shape/visibility support. [ ] Platform:
//  Missing clipboard support (not supported by Glut). [ ] Platform: Missing
//  gamepad support.

// You can use unmodified imgui_impl_* files in your project. See examples/
// folder for examples of using this. Prefer including the entire imgui/
// repository into your project (either as a copy or as a submodule), and only
// build the backends you need. If you are new to Dear ImGui, read documentation
// from the docs/ folder + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2022-09-26: Inputs: Renamed ImGuiKey_ModXXX introduced in 1.87 to
//  ImGuiMod_XXX (old names still supported). 2022-01-26: Inputs: replaced
//  short-lived io.AddKeyModsEvent() (added two weeks ago) with io.AddKeyEvent()
//  using ImGuiKey_ModXXX flags. Sorry for the confusion. 2022-01-17: Inputs:
//  calling new io.AddMousePosEvent(), io.AddMouseButtonEvent(),
//  io.AddMouseWheelEvent() API (1.87+). 2022-01-10: Inputs: calling new
//  io.AddKeyEvent(), io.AddKeyModsEvent() + io.SetKeyEventNativeData() API
//  (1.87+). Support for full ImGuiKey range. 2019-04-03: Misc: Renamed
//  imgui_impl_freeglut.cpp/.h to imgui_impl_glut.cpp/.h. 2019-03-25: Misc: Made
//  io.DeltaTime always above zero. 2018-11-30: Misc: Setting up
//  io.BackendPlatformName so it can be displayed in the About Window.
//  2018-03-22: Added GLUT Platform binding.

#include "imgui_impl_glut.h"
#include "../imgui.h" // IMGUI_IMPL_API
#include "stdio.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h> // Header File For The GLUT Library
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505) // unreferenced local function has been removed
                                // (stb stuff)
#endif

static int g_Time = 0; // Current time, in milliseconds

// Glut has 1 function for characters and one for "special keys". We map the
// characters in the 0..255 range and the keys above.
static ImGuiKey ImGui_ImplGLUT_KeyToImGuiKey(int key) {
  unsigned char ckey = (unsigned char)key;

  printf("%c\n", ckey);
  switch (ckey) {
  case 'e': /* delta, triangle */
    return ImGuiKey_UpArrow;
    break;
  case 'f': /* round */
    return ImGuiKey_RightArrow;
    break;
  case 'd': /* square*/
    return ImGuiKey_LeftArrow;
    break;
  case 'g':
    return ImGuiKey_DownArrow;
    break;
  case 's': /* select */
      return ImGuiKey_Enter;

    break;
  case 'a': /* startbutton */
            /* shut down our window */
            // but don't exist in pspgl
            //		glutDestroyWindow(window);  @@@

    /* exit the program...normal termination. */
  default:

      ;
  }
}

bool ImGui_ImplGLUT_Init() {
  ImGuiIO &io = ImGui::GetIO();

#ifdef FREEGLUT
  io.BackendPlatformName = "imgui_impl_glut (freeglut)";
#else
  io.BackendPlatformName = "imgui_impl_glut";
#endif
  g_Time = 0;

  return true;
}

static void joystick(unsigned int buttonMask, int x, int y, int z) {
  glClearColor(x * 1.0f / 2000.0f + 0.5f, y * 1.0f / 2000.0f + 0.5f, 1.0f,
               1.0f);
  glutPostRedisplay();
}

static void debug_test(unsigned char c, int x, int y) {
  printf("debug_test\n");
}

void ImGui_ImplGLUT_InstallFuncs() {
  printf("ImGui_ImplGLUT_InstallFuncs\n");
  glutJoystickFunc(joystick, 0);

  glutReshapeFunc(ImGui_ImplGLUT_ReshapeFunc);
   glutMotionFunc(ImGui_ImplGLUT_MotionFunc);
   glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
   glutMouseFunc(ImGui_ImplGLUT_MouseFunc);
#ifdef __FREEGLUT_EXT_H__
  glutMouseWheelFunc(ImGui_ImplGLUT_MouseWheelFunc);
#endif
  //glutKeyboardUpFunc(debug_test);
  //glutKeyboardFunc(debug_test);

   glutKeyboardUpFunc(ImGui_ImplGLUT_KeyboardUpFunc);
   glutKeyboardFunc(ImGui_ImplGLUT_KeyboardFunc);

  glutSpecialFunc(ImGui_ImplGLUT_SpecialFunc);
  glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
}

void ImGui_ImplGLUT_Shutdown() {}

void ImGui_ImplGLUT_NewFrame() {
  // Setup time step
  ImGuiIO &io = ImGui::GetIO();
  int current_time = 0; // glutGet(GLUT_ELAPSED_TIME);
  int delta_time_ms = (current_time - g_Time);
  if (delta_time_ms <= 0)
    delta_time_ms = 1;
  io.DeltaTime = delta_time_ms / 1000.0f;
  g_Time = current_time;

  // Start the frame
  ImGui::NewFrame();
}

static void ImGui_ImplGLUT_UpdateKeyModifiers() {
  /*ImGuiIO& io = ImGui::GetIO();
  int glut_key_mods = glutGetModifiers();
  io.AddKeyEvent(ImGuiMod_Ctrl, (glut_key_mods & GLUT_ACTIVE_CTRL) != 0);
  io.AddKeyEvent(ImGuiMod_Shift, (glut_key_mods & GLUT_ACTIVE_SHIFT) != 0);
  io.AddKeyEvent(ImGuiMod_Alt, (glut_key_mods & GLUT_ACTIVE_ALT) != 0);*/
}

static void ImGui_ImplGLUT_AddKeyEvent(ImGuiKey key, bool down,
                                       int native_keycode) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddKeyEvent(key, down);
  io.SetKeyEventNativeData(key, native_keycode,
                           -1); // To support legacy indexing (<1.87 user code)
}

void ImGui_ImplGLUT_KeyboardFunc(unsigned char c, int x, int y) {
  printf("ImGui_ImplGLUT_KeyboardFunc\n");

  // Send character to imgui
  printf("char_down_func %d '%c'\n", c, c);
  ImGuiIO &io = ImGui::GetIO();
  if (c >= 32)
    io.AddInputCharacter((unsigned int)c);

  ImGuiKey key = ImGui_ImplGLUT_KeyToImGuiKey(c);
  ImGui_ImplGLUT_AddKeyEvent(key, true, c);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x;
  (void)y; // Unused
}

void ImGui_ImplGLUT_KeyboardUpFunc(unsigned char c, int x, int y) {
  printf("char_up_func %d '%c'\n", c, c);
  ImGuiKey key = ImGui_ImplGLUT_KeyToImGuiKey(c);
  ImGui_ImplGLUT_AddKeyEvent(key, false, c);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x;
  (void)y; // Unused
}

void ImGui_ImplGLUT_SpecialFunc(int key, int x, int y) {
  printf("SPECIAL key_down_func %d %c\n", key+256, key+256);
  ImGuiKey imgui_key = ImGui_ImplGLUT_KeyToImGuiKey(key + 256);
  ImGui_ImplGLUT_AddKeyEvent(imgui_key, true, key + 256);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x;
  (void)y; // Unused
}

void ImGui_ImplGLUT_SpecialUpFunc(int key, int x, int y) {
  printf("SPECIAL key_up_func %d %c\n", key + 256, key + 256);
  ImGuiKey imgui_key = ImGui_ImplGLUT_KeyToImGuiKey(key + 256);
  ImGui_ImplGLUT_AddKeyEvent(imgui_key, false, key + 256);
  ImGui_ImplGLUT_UpdateKeyModifiers();
  (void)x;
  (void)y; // Unused
}

void ImGui_ImplGLUT_MouseFunc(int glut_button, int state, int x, int y) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);
  int button = -1;
  if (glut_button == GLUT_LEFT_BUTTON)
    button = 0;
  if (glut_button == GLUT_RIGHT_BUTTON)
    button = 1;
  if (glut_button == GLUT_MIDDLE_BUTTON)
    button = 2;
  if (button != -1 && (state == GLUT_DOWN || state == GLUT_UP))
    io.AddMouseButtonEvent(button, state == GLUT_DOWN);
}

#ifdef __FREEGLUT_EXT_H__
void ImGui_ImplGLUT_MouseWheelFunc(int button, int dir, int x, int y) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);
  if (dir != 0)
    io.AddMouseWheelEvent(0.0f, dir > 0 ? 1.0f : -1.0f);
  (void)button; // Unused
}
#endif

void ImGui_ImplGLUT_ReshapeFunc(int w, int h) {
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)w, (float)h);
}

void ImGui_ImplGLUT_MotionFunc(int x, int y) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddMousePosEvent((float)x, (float)y);
}
