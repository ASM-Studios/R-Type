#include "Texture.hpp"
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/typedefs.hpp>
#include <format>

Texture::LoadFailed::LoadFailed(const std::string& virtualPath) :
    _message(std::format("Failed to load image at {0}", virtualPath)) {}

const char *Texture::LoadFailed::what() const noexcept {
    return this->_message.c_str();
}

void Texture::_loadSize() {
    try {
        boost::gil::rgba8_image_t image;
        boost::gil::read_image(this->_realPath, image, boost::gil::png_tag());
        auto view = boost::gil::view(image);
        this->_size = std::make_pair((view.width() / this->_col) * this->_scale, (view.height() / this->_row) * this->_scale);
    } catch (std::ios_base::failure& e) {
        throw LoadFailed(this->_initialPath);
    }
}

Texture::Texture(const std::string& path, int row, int col, float scale) :
    _initialPath(std::move(path)),
    _virtualPath(std::move(path)),
    _row(row),
    _col(col),
    _scale(scale),
    _size(0, 0) {
    std::replace(this->_virtualPath.begin(), this->_virtualPath.end(), '.', '/');
    this->_realPath = std::format("./assets/textures/{0}.png", this->_virtualPath);
    this->_loadSize();
}

int Texture::getRow() const {
    return this->_row;
}

void Texture::setRow(int row) {
    this->_row = row;
    this->_loadSize();
}

int Texture::getCol() const {
    return this->_col;
}

void Texture::setCol(int col) {
    this->_col = col;
    this->_loadSize();
}

float Texture::getScale() const {
    return this->_scale;
}

void Texture::setScale(float scale) {
    this->_scale = scale;
    this->_loadSize();
}

std::pair<int, int> Texture::getSize() const {
    return this->_size;
}
