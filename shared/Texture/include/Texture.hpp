#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <exception>
#include <string>

class Texture {
    public:
        class LoadFailed final : public std::exception {
            private:
                std::string _message;

            public:
                explicit LoadFailed(const std::string& virtualPath);
                ~LoadFailed() override = default;

                [[nodiscard]] const char *what() const noexcept override;
        };

    private:
        std::string _initialPath;
        std::string _virtualPath;
        int _row;
        int _col;
        float _scale;
        std::pair<int, int> _size; // WIDTH * HEIGHT
        std::string _realPath;
        int _id;

        void _loadSize();

    public:
        explicit Texture(const std::string& path, int row, int col, float scale);
        ~Texture() = default;

        [[nodiscard]] int getRow() const;
        void setRow(int row);

        [[nodiscard]] int getCol() const;
        void setCol(int col);

        [[nodiscard]] float getScale() const;
        void setScale(float scale);

        [[nodiscard]] std::pair<int, int> getSize() const;

        int getTextureId() const;
        void setTexture(int textureId);
};
