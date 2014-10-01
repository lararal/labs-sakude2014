#include "stdafx.h"
#include "Entity.h"

Entity::Entity(DrawerAdapter adapter)
{
	this->adapter = adapter;
}

void Entity::Draw() {

}

void Entity::Fill() {

}

void Entity::DoTransformation(Matrix2D M){

}

Entity::~Entity()
{
}

