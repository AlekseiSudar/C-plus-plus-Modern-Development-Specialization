#include "Common.h"
#include <stdexcept>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
class Shape : public IShape {
public:
    // Возвращает точную копию фигуры.
    // Если фигура содержит текстуру, то созданная копия содержит ту же самую
    // текстуру. Фигура и её копия совместно владеют этой текстурой.
    Shape() : size_({0,0}), position_({0,0}), texture_(nullptr) {}

    virtual unique_ptr<IShape> Clone() const override = 0;

    void SetPosition(Point point) override { position_ = point; }
    Point GetPosition() const  override { return position_; }

    void SetSize(Size size) override { size_ = size; }
    Size GetSize() const override { return size_; }

    void SetTexture(std::shared_ptr<ITexture> texture) override { texture_ = texture; }
    ITexture* GetTexture() const override { return texture_.get(); }

    void Draw(Image& img) const override {

        auto IsPointInTexture = [this](Point p){
            if (!texture_) return false;

            Size size = texture_->GetSize();
            return p.x < size.width && p.y < size.height;
        };

        int X = img.empty() ? 0 : img[0].size();
        int Y = img.size();

        for (int x = 0; x < X; x++){
            for (int y = 0; y < Y; y++){
                Point p = { x - position_.x , y - position_.y };

                if (IsPointInShape(p))
                    if (IsPointInTexture(p))
                        img[y][x] = texture_->GetImage()[p.y][p.x];
                    else
                        img[y][x] = '.';
            }
        }
    }
private:
    virtual bool IsPointInShape(Point p) const = 0;
protected:
    Size size_;
    Point position_;
    shared_ptr<ITexture> texture_;
};



class Rectangle : public Shape{
public:
    unique_ptr<IShape> Clone() const{
        return make_unique<Rectangle>(*this);
    }
private:
    bool IsPointInShape(Point p) const override {
        return p.x >= 0 && p.y >= 0 && p.x < size_.width && p.y < size_.height;
    }
};

class Ellipse : public Shape{
public:
    unique_ptr<IShape> Clone() const{
        return make_unique<Ellipse>(*this);
    }
private:
    bool IsPointInShape(Point p) const override {
        return IsPointInEllipse(p, GetSize());
    }
};



// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    switch (shape_type) {
    case ShapeType::Rectangle:
        return make_unique<Rectangle>();
    case ShapeType::Ellipse:
        return make_unique<Ellipse>();
    }
    return nullptr;
}