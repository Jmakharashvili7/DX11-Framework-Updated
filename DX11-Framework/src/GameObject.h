#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include "structs.h"
#include "Appearance.h"

class Transform;

using namespace DirectX;
using namespace std;

class GameObject
{

protected:
	string m_Type;
	Transform* m_Transform;
	Appearance* m_Appearance;
public:
	GameObject(string type, Geometry geometry, Material* material);
	~GameObject();

	// returns a string of object type
	inline string GetType() const { return m_Type; }

	inline Transform* GetTransform() { return m_Transform; }

	inline Appearance* GetAppearance() const { return m_Appearance; }

	// Update function which sets the world matrix and takes in deltaTime
	void Update(const float dt);

	// Function for drawing the object
	void Draw(XMMATRIX& worldMatrix, ConstantBuffer& buffer, ID3D11Buffer* constBuffer, ID3D11DeviceContext* immediateContext);
};
