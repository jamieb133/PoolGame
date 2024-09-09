#include "Application.h"

int main()
{
    Application app;

    Application_Init(&app, 1200, 600, "Snookered!");
    Application_Run(&app);
}
