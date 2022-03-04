#pragma once
#include "GameObject.h"

using namespace DirectX;

class Transform
{
private:
	GameObject* m_Parent;
	XMFLOAT4X4 m_World;

	Vector3* m_Position;
	Vector3* m_Rotation;
	Vector3* m_Scale;
public:
	Transform() {}
	Transform(GameObject* parent, Vector3 position, Vector3 rotation, Vector3 scale);
	~Transform();

	void Update();

	// Get and set world matrix
	inline const XMFLOAT4X4* GetWorld() { return &m_World; }
	inline void SetWorld(XMFLOAT4X4 world) { m_World = world; }

	// Get and set parent
	inline const GameObject* GetParent() { return m_Parent; }
	inline void SetParent(GameObject* parent) { m_Parent = parent; }

	// Setters and Getters for position
	void SetPosition(Vector3 position) { m_Position = &position; }
	void SetPosition(float x, float y, float z) { m_Position->x = x; m_Position->y = y; m_Position->z = z; }

	Vector3* GetPosition() const { return m_Position; }

	// Setters and getters for scale
	void SetScale(Vector3 scale) { m_Scale = &scale; }
	void SetScale(float x, float y, float z) { m_Scale->x = x; m_Scale->y = y; m_Scale->z = z; }

	Vector3* GetScale() const { return m_Scale; }

	// Setters and getters for rotation
	void SetRotation(Vector3 rotation) { m_Rotation = &rotation; }
	void SetRotation(float x, float y, float z) { m_Rotation->x = x; m_Rotation->y = y; m_Rotation->z = z; }

	Vector3* GetRotation() const { return m_Rotation; }
};

