#pragma once

#include <utility>
#include <vector>

#include "components/simple_scene.h"
#include "Square.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include "Star.h"

namespace m1
{
   	class Tema1 : public gfxc::SimpleScene
	{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		glm::mat3 getModelMatrix(float scaleX, float scaleY, float translateX, float translateY, float scale_factor, float scale_factorY = 1);
		std::pair<float, float> getRandomCoordinates();
		bool mouseInRectangle(float mouseX, float mouseY, std::pair<float, float> center, float length, float width);
		void checkRhombusCollision (int scale_factor);
		void spawnAttackingStars(float deltaTimeSeconds);
		glm::mat3 getModelMatrixAngular(float scaleX, float scaleY, float translateX, float translateY,
										float scale_factorX, float scale_factorY, float radians);
		void checkAttackingStarCollision (int scale_factor);

	protected:
		int lives;
		int currency;
		int time_between_shots = 3;
		
		float pink_star_scale = 0.02;
		float rhombus_square_scale = 0.1;

		// size (pixels) of the original mesh (unscaled) on each axis
		float pink_star_sizeX = 3.4;
		float pink_star_sizeY = 3.4;

		// time interval (seconds) between appearances of pink stars
		int timeUntillNextStars;
		
		// time elapsed (Seconds) since last set of stars appeared
		float timeSinceLastStars;

		glm::mat3 modelMatrix;
		std::vector<std::pair<float,float>> pink_stars_pos;

		// stores the positions of clear squares with rhombuses inside
		std::vector<std::pair<float, float>> rhombus_shop;
		std::vector<int> rhombus_prices;

		// hexagon wannabe static members
		float scaleSizeHexagon = 0.035;
		float timeSinceLastSpawnHexagon = 0;
		int timeBetweenSpawnsHexagon = 5;
		std::vector<float> yCoordLinesHexagon;

		std::vector<Square> squares;
		std::vector<Rhombus> rhombuses;
		std::vector<Hexagon> hexagons;
		std::vector<Star> stars;
	};
}   // namespace m1
