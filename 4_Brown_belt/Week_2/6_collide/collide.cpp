#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;
using namespace geo2d;

template <typename T>
struct Collider : public GameObject {
    bool Collide(const GameObject& that) const final {
        return that.CollideWith(static_cast<const T&>(*this));
    }
};



#define METHOD(type) \
    bool CollideWith(const type& that) const;

#define METHODS()     \
    METHOD(Unit);     \
    METHOD(Building); \
    METHOD(Tower);    \
    METHOD(Fence);

#define CLASS(type, geom)                                     \
    class type : public Collider<type> {                      \
    public:                                                   \
        explicit type(geom geometry) : geometry_(geometry) {} \
        const geom& GetGeometry() const { return geometry_; } \
        METHODS();                                            \
    private:                                                  \
        geom geometry_;                                       \
    };

CLASS(Unit, Point);
CLASS(Building, Rectangle);
CLASS(Tower, Circle);
CLASS(Fence, Segment);



#define COLLEDE_WITH(type1, type2)                            \
    bool type1::CollideWith(const type2& that) const{         \
        return geo2d::Collide(geometry_, that.GetGeometry()); \
    }

#define COLLEDE_WITH1(type)       \
    COLLEDE_WITH(type, Unit);     \
    COLLEDE_WITH(type, Building); \
    COLLEDE_WITH(type, Tower);    \
    COLLEDE_WITH(type, Fence);

COLLEDE_WITH1(Unit);
COLLEDE_WITH1(Building);
COLLEDE_WITH1(Tower);
COLLEDE_WITH1(Fence);



bool Collide(const GameObject& first, const GameObject& second) {
    return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
    // Юнит-тест моделирует ситуацию, когда на игровой карте уже есть какие-то объекты,
    // и мы хотим добавить на неё новый, например, построить новое здание или башню.
    // Мы можем его добавить, только если он не пересекается ни с одним из существующих.

    const vector<shared_ptr<GameObject>> game_map = {
        make_shared<Unit>(Point{3, 3}),
        make_shared<Unit>(Point{5, 5}),
        make_shared<Unit>(Point{3, 7}),
        make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
        make_shared<Tower>(Circle{Point{9, 4}, 1}),
        make_shared<Tower>(Circle{Point{10, 7}, 1}),
        make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
    };

    for (size_t i = 0; i < game_map.size(); ++i) {
        Assert(
            Collide(*game_map[i], *game_map[i]),
            "An object doesn't collide with itself: " + to_string(i)
        );

        for (size_t j = 0; j < i; ++j) {
            Assert(
                !Collide(*game_map[i], *game_map[j]),
                "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
            );
        }
    }

    auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
    ASSERT(!Collide(*new_warehouse, *game_map[0]));
    ASSERT( Collide(*new_warehouse, *game_map[1]));
    ASSERT(!Collide(*new_warehouse, *game_map[2]));
    ASSERT( Collide(*new_warehouse, *game_map[3]));
    ASSERT( Collide(*new_warehouse, *game_map[4]));
    ASSERT(!Collide(*new_warehouse, *game_map[5]));
    ASSERT(!Collide(*new_warehouse, *game_map[6]));

    auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
    ASSERT(!Collide(*new_defense_tower, *game_map[0]));
    ASSERT(!Collide(*new_defense_tower, *game_map[1]));
    ASSERT(!Collide(*new_defense_tower, *game_map[2]));
    ASSERT( Collide(*new_defense_tower, *game_map[3]));
    ASSERT( Collide(*new_defense_tower, *game_map[4]));
    ASSERT(!Collide(*new_defense_tower, *game_map[5]));
    ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestAddingNewObjectOnMap);
    return 0;
}
