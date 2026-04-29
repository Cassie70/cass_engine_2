namespace cass {
    struct WindowProperties {
        unsigned int Width = 1280;
        unsigned int Height = 1280;
        std::string Title = "Application";
        bool VSync = false;

        bool Fullscreen = false;
        bool Resizable = true;
        bool Decorated = true;
        bool Maximized = false;
        int PosX = -1;
        int PosY = -1;
    };
}