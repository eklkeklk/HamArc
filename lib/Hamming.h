#include "ArgumentParsing.h"

std::vector<bool> EncodeHamming(const std::vector<bool>& message);

std::vector<bool> DecodeHamming(std::vector<bool>& encoded);

char ConvertBoolVectorToChar(const std::vector<bool>& boolVector);