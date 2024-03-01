#pragma once

namespace m1
{
    class Square
    {
        private:
            float centerScaleX, centerScaleY;
            float sizeScale; // should be 0.1
            bool occupied;
        public:
            Square(float centerScaleX, float centerScaleY, float sizeScale, bool occupied = false) {
                this->centerScaleX = centerScaleX;
                this->centerScaleY = centerScaleY;
                this->sizeScale = sizeScale;
                this->occupied = occupied;
            }

            bool isOccupied() {
                return this->occupied;
            }

            float getCenterScaleX() {
                return this->centerScaleX;
            }

            float getCenterScaleY() {
                return this->centerScaleY;
            }

            float getSizeScale() {
                return this->sizeScale;
            }

            void setOccupied(bool status) {
                this->occupied = status;
            }
    };
}