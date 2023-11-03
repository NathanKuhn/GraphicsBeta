#include "TextureAtlas.h"

#include <iostream>
#include "stb/stb_image.h"

namespace Renderer {

	TextureAtlas::TextureAtlas(std::vector<std::string> texturePaths) :
		textureHandle{ 0 },
		atlasWidth{ (unsigned short) glm::ceil(glm::sqrt(texturePaths.size())) },
		scale{ 1.0f / atlasWidth },
		texWidth{ atlasWidth * TEXTURE_SIZE },
		texHeight{ atlasWidth * TEXTURE_SIZE } {

		unsigned char *data = _sew(texturePaths);

		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			free(data);
		}
		else {
			std::cout << "Could not generate texture atlas.\n";
		}
	}

	glm::vec2 TextureAtlas::getOffset(unsigned short blockId) const {
		int x = (blockId - 1) / atlasWidth;
		int y = (blockId - 1) % atlasWidth;
		return glm::vec2(x * scale, y * scale);
	}

	float TextureAtlas::getScale() const {
		return scale;
	}

	void TextureAtlas::cleanup() {
		glDeleteTextures(1, &textureHandle);
	}

	unsigned char* TextureAtlas::_sew(std::vector<std::string> texturePaths) {
		int numTextures = texturePaths.size();
		unsigned char* data = (unsigned char*)malloc(3 * TEXTURE_SIZE * TEXTURE_SIZE * atlasWidth * atlasWidth * sizeof(unsigned char));
		
		if (data == NULL) {
			std::cout << "Could not allocate texture data\n";
			return NULL;
		}

		for (int textureIndex = 0; textureIndex < numTextures; textureIndex++) {
			int width, height, channels;
			unsigned char* imageData = stbi_load(texturePaths.at(textureIndex).c_str(), &width, &height, &channels, 0);

			int atlasPixelIndex, imagePixelIndex;
			int atlasOffsetX = textureIndex / atlasWidth * TEXTURE_SIZE;
			int atlasOffsetY = textureIndex % atlasWidth * TEXTURE_SIZE;

			if (imageData && width == TEXTURE_SIZE && height == TEXTURE_SIZE && channels == 3) {
				for (int x = 0; x < TEXTURE_SIZE; x++) {
					for (int y = 0; y < TEXTURE_SIZE; y++) {
						imagePixelIndex = x * TEXTURE_SIZE + y;
						atlasPixelIndex = (x + atlasOffsetX) * TEXTURE_SIZE * atlasWidth + (atlasOffsetY + y);

						data[atlasPixelIndex * 3] = imageData[3 * imagePixelIndex];
						data[atlasPixelIndex * 3 + 1] = imageData[3 * imagePixelIndex + 1];
						data[atlasPixelIndex * 3 + 2] = imageData[3 * imagePixelIndex + 2];
					}
				}
			}

			stbi_image_free(imageData);
		}

		return data;
	}
}