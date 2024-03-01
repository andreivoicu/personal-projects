#pragma once

namespace m1
{
    class Hexagon
    {
        private:
            float centerScaleX;

            int color;

            int line;

            int lives = 3;

            bool fading;

            float sizeScale;
        public:
            Hexagon(int color, int line, float centerScaleX, bool fading = false, float sizeScale = 0.035) {
                this->color = color;
                this->line = line;
                this->centerScaleX = centerScaleX;
                this->fading = fading;
                this->sizeScale = sizeScale;
            }

        static int getRandomLine() {
            return std::rand() % 3;
        }

        static int getRandomColor() {
            return std::rand() % 4;
        }

        // returns an int between 4 and 7
        static int getRandomTimeBetweenSpawns() {
            return (std::rand() % 4 + 4);
        }

        // setters
        void setSizeScale(float scale) {
            this->sizeScale = scale;
        }

        void setFading(bool fading) {
            this->fading = fading;
        }

        void setCenterScaleX(float x) {
            this->centerScaleX = x;
        }

        void setColor(int color) {
            if (color <= 3)
                this->color = 3;
            else
                printf("Invalid color given!");
        }

        void setLine(int line) {
            if (line <= 2) 
                this->line = line;
            else
                printf("Invalid line given!");
        }

        void updateLives() {
            this->lives--;
        }

        // getters
        float getSizeScale() {
            return this->sizeScale;
        }
        bool isFading() {
            return this->fading;
        }

        float getCenterScaleX() {
            return this->centerScaleX;
        }

        int getColor() {
            return this->color;
        }

        int getLine() {
            return this->line;
        }

        int getLives() {
            return this->lives;
        }



    };
}