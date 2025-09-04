// Dear ImGui: standalone example application for SDL2 + SDL_Renderer
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important to understand: SDL_Renderer is an _optional_ component of SDL2.
// For a multi-platform app consider using e.g. SDL+DirectX on Windows and SDL+OpenGL on Linux/OSX.

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_syswm.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

static SDL_HitTestResult WindowHitTest(SDL_Window *window, const SDL_Point *area, void *data)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // printf("HIT TEST %d %d\n", area->x, area->y);

    int border = 6;

    if (area->x < border && area->y < border)
        return SDL_HITTEST_RESIZE_TOPLEFT;
    if (area->x >= w - border && area->y < border)
        return SDL_HITTEST_RESIZE_TOPRIGHT;
    if (area->x >= w - border && area->y >= h - border)
        return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
    if (area->x < border && area->y >= h - border)
        return SDL_HITTEST_RESIZE_BOTTOMLEFT;

    if (area->x < border)
        return SDL_HITTEST_RESIZE_LEFT;
    if (area->x >= w - border)
        return SDL_HITTEST_RESIZE_RIGHT;
    if (area->y < border)
        return SDL_HITTEST_RESIZE_TOP;
    if (area->y >= h - border)
        return SDL_HITTEST_RESIZE_BOTTOM;

    if (area->y < 22)
    {
        SDL_Rect close_button    = {w-20-1, 4-1, 16+2, 14+2};
        SDL_Rect maximize_button = {w-40-1, 4-1, 16+2, 14+2};
        SDL_Rect minimize_button = {w-60-1, 4-1, 16+2, 14+2};

        if (SDL_PointInRect(area, &close_button))
        {
            return SDL_HITTEST_NORMAL;
        }

        if (SDL_PointInRect(area, &maximize_button))
        {
            return SDL_HITTEST_NORMAL;
        }

        if (SDL_PointInRect(area, &minimize_button))
        {
            return SDL_HITTEST_NORMAL;
        }

        return SDL_HITTEST_DRAGGABLE;
    }

    return SDL_HITTEST_NORMAL;
}

bool g_CollapsePressed;
bool g_MaximizePressed;

// Main code
int main(int, char**)
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    SDL_SetHint("SDL_BORDERLESS_WINDOWED_STYLE", "1");
    SDL_SetHint("SDL_BORDERLESS_RESIZABLE_STYLE", "1");

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE
                                                     | SDL_WINDOW_ALLOW_HIGHDPI
                                                     | SDL_WINDOW_BORDERLESS);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetWindowHitTest(window, WindowHitTest, nullptr);
    SDL_SetWindowResizable(window, SDL_TRUE);

    /*SDL_SysWMinfo info;
    SDL_GetWindowWMInfo(window, &info);

    SDL_ShowWindow(window);

    ::SetWindowPos(info.info.win.window, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

    ::ShowWindow(info.info.win.window, SW_SHOWDEFAULT);
    ::UpdateWindow(info.info.win.window);*/

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = false;
    bool show_main_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGuiWindowFlags main_window_flags = (ImGuiWindowFlags_NoResize
                                              | ImGuiWindowFlags_NoCollapse);

        ImGui::SetNextWindowPos({});
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Main Window###MAIN_WINDOW", &show_main_window, main_window_flags);
        {
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        // printf("%d RENDERING\n", rand());

        if (!show_main_window)
            done = true;

        if (g_CollapsePressed)
        {
            // printf("COLLAPSE\n");
            SDL_MinimizeWindow(window);
            g_CollapsePressed = false;
        }

        if (g_MaximizePressed)
        {
            // printf("MAXIMIZE\n");
            if (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) {
                SDL_RestoreWindow(window);
            } else {
                SDL_MaximizeWindow(window);
            }
            g_MaximizePressed = false;
        }
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
