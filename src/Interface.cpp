#include "Interface.hpp"

using namespace FrameWork::Assets;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace FrameWork
{
    void Interface::UpdateStyle()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowRounding = 16;
        style->WindowBorderSize = 0;
        style->WindowPadding = ImVec2(0, 0);
        style->ScrollbarSize = 2;
        style->FrameRounding = 10.0f;
        style->ScrollbarRounding = 12.0f;

        style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_SeparatorActive] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_SeparatorHovered] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_PopupBg] = ImColor(14, 14, 14);
        style->Colors[ImGuiCol_ScrollbarBg] = ImColor(0, 0, 0, 0);
        style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(183, 180, 255);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(183, 180, 255);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(183, 180, 255);

        style->Colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15);
        style->Colors[ImGuiCol_Border] = ImColor(24, 23, 25);

        style->Colors[ImGuiCol_TextDisabled] = ImColor(80, 80, 80);
    }

    int cont2 = 0;
    int count = 0;
    int tablogin = 0;
    float testSlider = 34, smooth = 1;
    int active_tab = 0;
    float tab_alpha = 0.f;
    float tab_add = 0.f;
    char Username[40] = "";
    char Password[10] = "";
    int textIndex = 0;
    const char* loadingText[] = {
        "Loading",
        "Loading.",
        "Loading..",
        "Loading..."
    };
    static std::string error_message = "";

    bool closebuttondokye(const char* str_id, const ImVec2& pos, ImVec2 size, ImU32 colorNormal, ImU32 colorHover, ImU32 colorClicked)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        const ImRect bb(pos, pos + size);
        ImRect bb_interact = bb;

        bool is_clipped = !ImGui::ItemAdd(bb_interact, ImGui::GetID(str_id));
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb_interact, ImGui::GetID(str_id), &hovered, &held);

        if (is_clipped)
            return pressed;

        ImU32 buttonColor = colorNormal;
        if (hovered)
            buttonColor = colorHover;
        if (held)
            buttonColor = colorClicked;

        ImVec2 center = bb.GetCenter();

        ImU32 backgroundColor = IM_COL32(17, 17, 17, 255);
        window->DrawList->AddRectFilled(bb.Min, bb.Max, backgroundColor, 5.0f);

        float cross_extent = g.FontSize * 0.5f * 0.7071f - 1.0f;
        ImU32 crossCol = IM_COL32(108, 108, 108, 255);
        center -= ImVec2(0.5f, 0.5f);
        window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent), center + ImVec2(-cross_extent, -cross_extent), crossCol, 1.0f);
        window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent), center + ImVec2(-cross_extent, +cross_extent), crossCol, 1.0f);

        return pressed;
    }

    void Interface::RenderGui()
    {
        static float alpha = 0.0f;
        static float textAlpha = 0.0f;
        static float elapsedTime = 0.0f;

        float deltaTime = ImGui::GetIO().DeltaTime;
        alpha = ImClamp(alpha + deltaTime * 1.5f, 0.0f, 1.0f);
        elapsedTime += deltaTime;

        if (CurrentTab == 0)
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(565, 385));
            ImGui::Begin("L", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            tab_alpha = ImClamp(tab_alpha + (3.f * deltaTime * (tablogin == active_tab ? 1.f : -1.f)), 0.f, 1.f);
            tab_add = ImClamp(tab_add + (175.f * deltaTime * (tablogin == active_tab ? 1.f : -1.f)), 0.f, 1.f);

            if (tab_alpha <= 0.01f && tab_add <= 0.01f)
                active_tab = tablogin;

            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style.Alpha);

            if (tablogin == 0)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 pos = ImGui::GetWindowPos();

                ImVec4 imageColor = ImVec4(1.0f, 1.0f, 1.0f, alpha);
                // drawList->AddImage(Assets::LogoLogin, pos + ImVec2(240, 146), pos + ImVec2(330, 234), ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(imageColor));
                ImGui::PopStyleVar();

                if (elapsedTime >= 2.0f)
                {
                    textAlpha = ImClamp(textAlpha + deltaTime * 1.5f, 0.0f, 1.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, textAlpha);

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 240);
                    float textWidth = ImGui::CalcTextSize(loadingText[textIndex]).x;
                    float regionWidth = ImGui::GetWindowContentRegionMax().x;
                    ImGui::SetCursorPosX((regionWidth - textWidth) * 0.5f);
                    ImGui::Text("%s", loadingText[textIndex]);

                    ImGui::PopStyleVar();

                    cont2++;
                    if (cont2 >= 35)
                    {
                        textIndex = (textIndex + 1) % (sizeof(loadingText) / sizeof(loadingText[0]));
                        cont2 = 0;
                    }
                }

                count++;
                if (count >= 700)
                {
                    tablogin = 1;
                }
            }
            else if (tablogin == 1)
            {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 pos = ImVec2(-15, -15);
                ImVec4 imageColor = ImVec4(1.0f, 1.0f, 1.0f, alpha);
                // drawList->AddImage(Assets::logopequena, pos + ImVec2(28, 27), pos + ImVec2(56, 54), ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(imageColor));

                ImVec2 pos1 = ImVec2(0, 48);
                ImVec2 pos2 = ImVec2(565, 48);
                ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
                drawList->AddLine(pos1, pos2, color, 1.0f);

                ImGui::SetCursorPos(ImVec2(260, 16));
                ImGui::Text("Bypass");

                if (closebuttondokye("fodase", ImVec2(528, 13), ImVec2(23, 23),
                    IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0)))
                {
                    exit(1);
                }

                ImVec2 crossPos = ImVec2(528, 13);
                ImVec2 crossSize = ImVec2(23, 23);
                ImVec2 center = crossPos + crossSize * 0.5f;
                float cross_extent = ImGui::GetFontSize() * 0.5f * 0.7071f - 1.0f;
                ImU32 crossCol = IM_COL32(255, 255, 255, 255);
                ImGui::GetWindowDrawList()->AddLine(center + ImVec2(+cross_extent, +cross_extent), center + ImVec2(-cross_extent, -cross_extent), crossCol, 1.0f);
                ImGui::GetWindowDrawList()->AddLine(center + ImVec2(+cross_extent, -cross_extent), center + ImVec2(-cross_extent, +cross_extent), crossCol, 1.0f);

                ImVec2 window_size = ImGui::GetWindowSize();

                float input_width = 220.0f;
                float input_x = (window_size.x - input_width) / 2.0f;
                ImGui::SetCursorPos(ImVec2(input_x, 170));
                ImGui::InputTextEx("Key", "License Key", Username, sizeof(Username), ImVec2(input_width, 40), ImGuiInputTextFlags_AllowTabInput);
                ImGui::SetCursorPos(ImVec2(input_x, 190));
                

                if (!error_message.empty()) {
                    ImGui::SetCursorPos(ImVec2(input_x, 220));
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_message.c_str());
                }

                float button_width = 240.0f;
                float button_x = (window_size.x - button_width) / 2.0f;
                ImGui::SetCursorPos(ImVec2(button_x, 269));
                if (ImGui::Button("Sign In", ImVec2(button_width, 40)))
                {
                    tablogin = 2;
                }


            }
            else if (tablogin == 2)
            {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 pos = ImVec2(-15, -15);
                ImVec4 imageColor = ImVec4(1.0f, 1.0f, 1.0f, alpha);
                // drawList->AddImage(Assets::logopequena, pos + ImVec2(28, 27), pos + ImVec2(56, 54), ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(imageColor));

                ImVec2 pos1 = ImVec2(0, 48);
                ImVec2 pos2 = ImVec2(565, 48);
                ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
                drawList->AddLine(pos1, pos2, color, 1.0f);

                ImGui::SetCursorPos(ImVec2(260, 16));
                ImGui::Text("Bypass");

                if (closebuttondokye("fodase", ImVec2(528, 13), ImVec2(23, 23),
                    ImColor(32, 32, 32, 255), ImColor(32, 32, 32, 255), ImColor(32, 32, 32, 255)))
                {
                    exit(1);
                }

                ImGui::SetCursorPos(ImVec2(306, 180));
                if (ImGui::Button("Clean", ImVec2(234, 40)))
                {
                }

                ImGui::SetCursorPos(ImVec2(306, 130));
                if (ImGui::Button("Inject", ImVec2(234, 40)))
                {
                }

                ImGui::SetCursorPos(ImVec2(306, 241));
                ImGui::Text("Expires");
                ImGui::SameLine(0, 10);
                ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), ": Lifetime");

                float nextY = 241 + ImGui::GetTextLineHeightWithSpacing() + 15;
                ImGui::SetCursorPosY(nextY);
                ImGui::SetCursorPosX(306);
                ImGui::Text("Status");
                ImGui::SameLine(0, 10);
                ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), ": Stable");

                nextY += ImGui::GetTextLineHeightWithSpacing() + 15;
                ImGui::SetCursorPos(ImVec2(306, nextY));
                ImGui::Text("Last Update");
                ImGui::SameLine(0, 10);
                ImGui::TextColored(ImVec4(1, 1, 1, 0.5f), ": 01/01/2025");

                ImVec2 pos5 = ImVec2(282, 57);
                ImVec2 pos6 = ImVec2(282, 57 + 315);
                ImU32 color2 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
                drawList->AddLine(pos5, pos6, color2, 2.0f);

                ImGui::SetCursorPos(ImVec2(20, 100));
                ImGui::CustomChild("Info : ", ImVec2(250, 200));
                {
                    ImGui::Text("- Use The Cleaner In To");
                    ImGui::Text("  Clean Cheat Never Leave");
                    ImGui::Text("  The Cheat Without Cleaning. ");
                }
                ImGui::EndCustomChild();
            }

            ImGui::PopStyleVar();
            Overlay.Mouse_Move();
            ImGui::End();
        }
    }
}