#include "utils.h"

#include <fstream>
#include <sstream>



namespace utils {

SwissArmyToolbox::SwissArmyToolbox()
{
}

std::string SwissArmyToolbox::loadTextFromFile(const std::string &filepath) const
{
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            return "<!DOCTYPE html><html lang=\"en\"><body><h1> ERROR 500 </h1><p> SERVER ERROR: CAN NOT LOAD RESOURCE!</p></body></html>";
        }
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
}

std::string SwissArmyToolbox::loadImageFromFile(const std::string &filepath) const
{
        std::ifstream imageFile(filepath, std::ios::binary);
        std::ostringstream imageBuffer;
        imageBuffer << imageFile.rdbuf();
        return imageBuffer.str();
}

void SwissArmyToolbox::replaceWith(std::string &text, const std::string &original, const std::string &replacement) const
{
        size_t originalPosition = text.find(original);
        if (originalPosition != std::string::npos)
        {
            text.replace(originalPosition, original.size(), replacement);
        }
}


}

