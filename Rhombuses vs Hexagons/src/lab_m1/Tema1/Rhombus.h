#pragma once

namespace m1
{
    class Rhombus
    {
        private:
            float centerScaleX, centerScaleY;

            int color;

            // time elapsed since last projectile left the rhombus
            float timeSinceLastShot;

            // the square on which this rhombus is placed (the position of the square in vector)
            int squarePos;

            // if this rhombus is currently shooting
            bool active;

            // if this rhombus is in the proccess of Drag and Drop travel = ture
            // only the last element in the 'rhombuses' vector can have travel == true,
            // as only one rhombus can travel at a certain point in time
            bool travel;

            float sizeScale;

            // set to true if the current rhombus is fading
            // (decreasing size following an attack by a hexagon)
            // set to false by default
            bool fading;
        public:
            Rhombus(int color, float timeSinceLastShot, bool active, bool travel, float centerScaleX,
                    float centerScaleY, float sizeScale = 0.1, bool fading = false) {
                this->color = color;
                this->timeSinceLastShot = timeSinceLastShot;
                this->active = active;
                this->travel = travel;
                this->centerScaleX = centerScaleX;
                this->centerScaleY = centerScaleY;
                this->sizeScale = sizeScale;
                this->fading = fading;
            }

            // setters
            void setFading (bool fading) {
                this->fading = fading;
            }

            void setSizeScale(float scale) {
                this->sizeScale = scale;
            }

            void setSquarePos (int pos) {
                this->squarePos = pos;
            }

            void setCenterScaleX(float x) {
                this->centerScaleX = x;
            }

            void setCenterScaleY(float y) {
                this->centerScaleY = y;
            }

            void setTravel(bool travel) {
                this->travel = travel;
            }

            void disable() {
                this->active = false;
            }

            void enable() {
                this->active = true;
            }

            void updateTimer(float time) {
                this->timeSinceLastShot += time;
            }

            void restartTimer() {
                this->timeSinceLastShot = 0;
            }

            // getters
            float getSizeScale() {
                return this->sizeScale;
            }

            float getCenterScaleX() {
                return this->centerScaleX;
            }

            float getCenterScaleY() {
                return this->centerScaleY;
            }

            int getColor() {
                return this->color;
            }

            float getTimeSinceLastShot() {
                return this->timeSinceLastShot;
            }

            int getSquarePos() {
                if (this->travel) {
                    return -1;
                }
                return this->squarePos;
            }

            bool isActive() {
                return this->active;
            }

            bool isTravelling() {
                return this->travel;
            }

            bool isFading() {
                return this->fading;
            }
    };
}