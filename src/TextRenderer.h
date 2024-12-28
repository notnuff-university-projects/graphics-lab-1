#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include <map>
#include <string>
#include <glm/vec3.hpp>

#include "Shader.h"

#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    FT_Pos Advance;    // Offset to advance to next glyph
};


class TextRenderer {
    public:

    bool Init();

    void RenderText(Shader &shader, std::string text,
        float x, float y,
        float scale, unsigned int VAO, unsigned int VBO);

protected:
    std::map<char, Character> characters_;
};



#endif //TEXTRENDERER_H
