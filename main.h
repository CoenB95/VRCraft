#ifndef MAIN_H
#define MAIN_H

void color3fHue(GLfloat hue);
void onDraw();
void onIdle();
void onKeyPressed(unsigned char key, int x, int y);
void onKeyCodePressed(int keyCode, int x, int y);
void onMouseMove(int x, int y);
void onMousePressed(int button, int state, int x, int y);
void onResize(int width, int height);

#endif // !MAIN_H
