// Input Helpers //
void keysPressed(unsigned char key, int x, int y);
void keysUp(unsigned char key, int x, int y);
void mouse(int mouse_button, int state, int x, int y);

// Output Helpers //
void printInstructions();

// Scene Update Helpers //
void update();

// View Helpers
void SetOrthographicProjection(int w,int h);
void SetPerspectiveProjection(int w,int h);
void Set3rdPersonProjection(int w, int h);
void drawWorld();