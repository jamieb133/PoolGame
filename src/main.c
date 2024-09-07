#include "Application.h"

int main()
{
    Application app;
    Application_Init(&app, 1200, 600, "Snookered!");

#ifdef DEBUG
    // Apply a simulated cue stroke.
    app.white_ball->vx -= 500.0f;  
#endif

    Application_Run(&app);
}
