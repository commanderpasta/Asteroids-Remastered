#pragma once

#include "gl/Shader.h"
#include "gl/Texture.h"

#include "BaseDataView.h"
#include "ActorDataType.h"

#include <string>
#include <map>
#include <memory>
#include <vector>

/**
 * A single <Character> that belongs to a line of text.
 *
 * Contains render data.
 */
class Character {
public:
	Shader shader; /**< The <Shader> program for this <Character>. */
	std::shared_ptr<BaseDataView> data; /**< The OpenGL buffer data for this <Character>. */

	Character(std::shared_ptr<BaseDataView> data, std::string shaderPath, float x, float y);
	~Character();

	Character(Character&& other) noexcept
		: shader(std::move(other.shader)), data(std::move(other.data)) {
		other.shader.m_RendererID = 0;
	}

	Character& operator=(Character&& other) noexcept {
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->shader.m_RendererID = 0;
			std::swap(this->shader, other.shader);
		}
	}

	void setPosition(float position[2], float angle);
	void setResolution(float x, float y);
};

/**
 * A line of <Text> that is displayed on screen.
 * 
 * Its render data is described separately for each <Character>.
 */
class Text {
public:
	unsigned int id; /**< The id of this UI text. */
	std::string text; /**< The text that it displays. */
	std::vector<Character> characters; /**< The characters the <text> consists of.*/
	static inline std::shared_ptr<Texture> texture; /**< The texture bitmap for the font of this text. */

	Text(unsigned int id, std::string text, float x, float y, float windowX, float windowY);

	static void setup();
};

/**< A map that represents the coordinates of the character tile map used for this game's font bitmap. */ 
inline std::map<char, std::pair<unsigned int, unsigned int>> charCoordinates = 
{{' ', {0, 0}},{'!', {0, 1}},{'"', {0, 2}},{'#', {0, 3}},{'$', {0, 4}},{'%', {0, 5}},{'&', {0, 6}},{'\'', {0, 7}},
{'(', {1, 0}},{')', {1, 1}},{'*', {1, 2}},{'+', {1, 3}},{',', {1, 4}},{'-', {1, 5}},{'.', {1, 6}},{'/', {1, 7}}, 
{'0', {2, 0}},{'1', {2, 1}},{'2', {2, 2}},{'3', {2, 3}},{'4', {2, 4}},{'5', {2, 5}},{'6', {2, 6}},{'7', {2, 7}}, 
{'8', {3, 0}},{'9', {3, 1}},{':', {3, 2}},{';', {3, 3}},{'<', {3, 4}},{'=', {3, 5}},{'>', {3, 6}},{'?', {3, 7}}, 
{'@', {4, 0}},{'A', {4, 1}},{'B', {4, 2}},{'C', {4, 3}},{'D', {4, 4}},{'E', {4, 5}},{'F', {4, 6}},{'G', {4, 7}}, 
{'H', {5, 0}},{'I', {5, 1}},{'J', {5, 2}},{'K', {5, 3}},{'L', {5, 4}},{'M', {5, 5}},{'N', {5, 6}},{'O', {5, 7}}, 
{'P', {6, 0}},{'Q', {6, 1}},{'R', {6, 2}},{'S', {6, 3}},{'T', {6, 4}},{'U', {6, 5}},{'V', {6, 6}},{'W', {6, 7}}, 
{'X', {7, 0}},{'Y', {7, 1}},{'Z', {7, 2}},{'[', {7, 3}},{'\\', {7, 4}},{']', {7, 5}},{'^', {7, 6}},{'_', {7, 7}}, };

