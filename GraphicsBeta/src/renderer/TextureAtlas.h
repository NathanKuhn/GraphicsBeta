#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "GL/glew.h"

#define TEXTURE_SIZE 16

namespace Renderer {
	class TextureAtlas {

	private:
		GLuint textureHandle;
		const unsigned short atlasWidth;
		const float scale;
		int texWidth, texHeight;

		/// <summary>
		/// Will "sew" many different square images of size TEXTURE_SIZE together into a single large square texture.
		/// </summary>
		unsigned char *_sew(std::vector<std::string> texturePaths);

	public:
		TextureAtlas(std::vector<std::string> texturePaths);

		glm::vec2 getOffset(unsigned short blockId) const;
		float getScale() const;

		void cleanup();
	};
}

