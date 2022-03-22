#pragma once

namespace Global_Vars
{
	char cSigBuf[1024] = "";
	std::vector <std::string> sSigBufArray = {};
	std::vector <std::string> sSigMaskBufArray = {};
	std::vector <std::string> sSigIDABufArray = {};
	std::string sSigBuf = "";
	std::string sSigMask = "";
	bool bIDASig = "";
	bool bConvertSigSelected = false;
	bool bCopySigSelected = false;
	bool bDisplayConvertedSig = false;
	bool bConvertedSigDisplayed = false;
};