#pragma once

namespace m1
{
    class Star
    {
        private:
            float angle;
            float centerScaleX, centerScaleY;
            float sizeScale;
            int color;
        public:
            Star(float centerScaleX, float centerScaleY, int color, float sizeScale = 0.015,
                float angle = 0) {
                
                this->centerScaleX = centerScaleX;
                this->centerScaleY = centerScaleY;
                this->color = color;
                this->sizeScale = sizeScale;
                this->angle = angle;
            }

            // setters
            void setAngle(float angle) {
                this->angle = angle;
            }

            void setCenterScaleX(float x) {
                this->centerScaleX = x;
            }

            void setCenterScaleY(float y) {
                this->centerScaleY = y;
            }

            void setSizeScale(float scale) {
                this->sizeScale = scale;
            }

            // getters
            int getColor() {
                return this->color;
            }

            float getAngle() {
                return this->angle;
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
    };
}