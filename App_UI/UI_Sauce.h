#pragma once
#include "../imgui/imgui_internal.h"
#include <string>
#include <vector>
#include <functional>
#include "../Global_Vars/Global_Vars.h"

#define MAX_RGB 255.0
#define HELPMARKERDESC(str) ImGui::SameLine(); ImGui::TextColored(ImColor(0, 130, 15, 255), "[ ? ]"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)
#define HELPMARKERDISC(str) ImGui::SameLine(); ImGui::TextColored(ImColor(0, 130, 15, 255), "[ * ]"); if (ImGui::IsItemHovered()) ImGui::SetTooltip(str)
#pragma warning(disable: 26812 26815)

namespace ImGui
{
	void Line(int newId)
	{
		std::string id = ("ImGui_Sauce_line_" + std::to_string(newId));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
		{
			ImGui::BeginChild(id.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 1), false);
			ImGui::Separator();
			ImGui::EndChild();
		}
		ImGui::PopStyleColor();
	}

	void Linevertical()
	{
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
	}

	void CenterText(const char* text, int lineId, bool separator)
	{
		if (text == nullptr)
			return;

		ImGui::Spacing();
		ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
		ImGui::Text(text);
		ImGui::Spacing();

		if (true == separator)
			Line(lineId);
	}

	void CenterTextEx(const char* text, float width, int lineId, bool separator)
	{
		if (text == nullptr)
			return;

		ImGui::Spacing();
		ImGui::SameLine((width / 2) - (ImGui::CalcTextSize(text).x / 2));
		ImGui::Text(text);
		ImGui::Spacing();

		if (true == separator)
			Line(lineId);
	}

	float GetX()
	{
		return ImGui::GetContentRegionAvail().x;
	}

	float GetY()
	{
		return ImGui::GetContentRegionAvail().y;
	} 

	void SubSectionGUITheme()
	{
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
		ImGuiStyle* Style = &ImGui::GetStyle();
		Style->WindowBorderSize = 4;
		Style->FrameBorderSize = 0;
		Style->ChildBorderSize = 4;
		Style->WindowRounding = 0;
		Style->ChildRounding = 0;
		Style->FrameRounding = 0;
		Style->ScrollbarSize = 0;
		Style->WindowPadding = ImVec2(0, 0);
		Style->FramePadding = ImVec2(0, 0);
		Style->ItemSpacing = ImVec2(0, 0);
		Style->ItemInnerSpacing = ImVec2(0, 0);
		Style->IndentSpacing = 0;
		Style->DisplayWindowPadding = ImVec2(0, 0);
		Style->Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		Style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		//Style->Colors[ImGuiCol_WindowBg] = ImColor(255, 255, 255);
		Style->Colors[ImGuiCol_CheckMark] = ImColor(15, 110, 15);
		Style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40, 155); // 40, 40, 40, 155
		Style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40, 255);
		Style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40, 255);
		Style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255);
		Style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255);
	}

#pragma region ASC Gui Setup
	void DisplayTopHeader()
	{
		ImGui::SetCursorPosY(10);
		CenterText("Axera Sig Converter", 1, false);
		ImGui::SetCursorPos(ImVec2(290, 8));
		if (ImGui::Button("Exit", ImVec2(50, 20)))
		{
			exit(0);
		}
	}

	std::string ConvertCharToStr(char* sig)
	{
		Global_Vars::sSigBuf = sig;
		return Global_Vars::sSigBuf;
	}

	void ConvertSig()
	{
		if (Global_Vars::sSigBuf.find("\\x") != std::string::npos)
			Global_Vars::bIDASig = false;
		else if (Global_Vars::sSigBuf.find("?") != std::string::npos)
			Global_Vars::bIDASig = true;

		if (Global_Vars::bIDASig)
		{
			while (Global_Vars::sSigBuf.find("?") != std::string::npos)
				Global_Vars::sSigBuf.replace(Global_Vars::sSigBuf.find("?"), 1, "00");
			while (Global_Vars::sSigBuf.find(" ") != std::string::npos)
				Global_Vars::sSigBuf.replace(Global_Vars::sSigBuf.find(" "), 1, "\\x");
			Global_Vars::sSigBuf.insert(0, "\\x");

			Global_Vars::sSigMask = "";

			for (size_t i = 1; i <= Global_Vars::sSigBuf.length() / 4; i++)
			{
				int32_t index = i * 4 - 1;

				if (Global_Vars::sSigBuf.at(index) == '0' && Global_Vars::sSigBuf.at(index - 1) == '0')
					Global_Vars::sSigMask += "?";
				else
					Global_Vars::sSigMask += "x";
			}

			Global_Vars::sSigBufArray.push_back(Global_Vars::sSigBuf);

			Global_Vars::sSigMaskBufArray.push_back(Global_Vars::sSigMask);
		}
		else if (!Global_Vars::bIDASig)
		{
			while (Global_Vars::sSigBuf.find("00") != std::string::npos)
				Global_Vars::sSigBuf.replace(Global_Vars::sSigBuf.find("00"), 2, "?");
			while (Global_Vars::sSigBuf.find("\\x") != std::string::npos)
				Global_Vars::sSigBuf.replace(Global_Vars::sSigBuf.find("\\x"), 2, " ");

			Global_Vars::sSigIDABufArray.push_back(Global_Vars::sSigBuf);
		}
	}

	void DisplayGenRandString()
	{
		SubSectionGUITheme();

		if (ImGui::BeginChild("DisplaySigConverter", ImVec2(350, 175), true, ImGuiWindowFlags_NoDecoration))
		{
			ImGui::SetCursorPosY(25);
			CenterText("[ Convert Sigs ]", 0, false);
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::Indent(15);
			ImGui::Text("Insert Sig  [ ");
			ImGui::Unindent(15);
			ImGui::SameLine();
			ImGui::PushItemWidth(175);
			ImGui::InputText("##InputSigToConvert", Global_Vars::cSigBuf, IM_ARRAYSIZE(Global_Vars::cSigBuf));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Text(" ]  ");
			ImGui::SameLine();
			if (ImGui::Button("Convert", ImVec2(60, 20)))
			{
				Global_Vars::bConvertSigSelected = true;
			}
			ImGui::NewLine();
			ImGui::Indent(125);
			if (ImGui::Button("Copy Sig", ImVec2(100, 20)))
			{
				Global_Vars::bCopySigSelected = true;
			}
			ImGui::Unindent(125);
			ImGui::NewLine();
			if (Global_Vars::bConvertSigSelected)
			{
				ConvertCharToStr(Global_Vars::cSigBuf);
				ConvertSig();
				if (!Global_Vars::bDisplayConvertedSig)
				{
					Global_Vars::bDisplayConvertedSig = true;
				}
				Global_Vars::bConvertSigSelected = false;
			}
			if (Global_Vars::bDisplayConvertedSig)
			{
				ImGui::SetNextWindowSize(ImVec2(800, 300), ImGuiCond_Once);
				ImGui::Begin("Sig Display", 0, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
				ImGui::SetCursorPos(ImVec2(15, 25));
				if (ImGui::Button("Clear", ImVec2(50, 20)))
				{
					Global_Vars::sSigBufArray.clear();
					Global_Vars::sSigMaskBufArray.clear();
					Global_Vars::sSigIDABufArray.clear();
				}
				if (Global_Vars::bCopySigSelected)
				{
					ImGui::LogToClipboard();
					Global_Vars::bCopySigSelected = false;
				}
				for (int i = 0; i < Global_Vars::sSigBufArray.size(); i++)
				{
					ImGui::CenterText(Global_Vars::sSigBufArray[i].c_str(), 0, false);
					ImGui::CenterText(Global_Vars::sSigMaskBufArray[i].c_str(), 0, false);
				}
				for (int i = 0; i < Global_Vars::sSigIDABufArray.size(); i++)
				{
					ImGui::CenterText(Global_Vars::sSigIDABufArray[i].c_str(), 0, false);
				}
				ImGui::End();
			}
		} ImGui::EndChild();
	}
#pragma endregion
}