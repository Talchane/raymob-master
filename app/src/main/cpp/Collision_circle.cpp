//
// Created by benfe on 14/09/2024.
//

#include "Collision_circle.hpp"

CollisionCircle::CollisionCircle()
{}

CollisionCircle::CollisionCircle(const Vector2 &_pos_, const float _rad_) : position(_pos_), radius(_rad_)
{}

void CollisionCircle::Draw() const
{
    DrawCircleV(position, radius, (Color){255, 50, 50, 150}); // Dessiner le cercle de collision
}

bool collisionCercleCercle(const CollisionCircle& C1, const CollisionCircle& C2)
{
    return (distance(C2.position, C1.position) <= (C1.radius + C2.radius));
}