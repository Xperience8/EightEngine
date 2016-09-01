#pragma once

struct PosVertex
{
	DirectX::XMFLOAT3 Position;

	PosVertex() { }
	PosVertex(float x, float y, float z) : Position(x, y, z) { }
};
struct PosTexVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};
struct PosColVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};
struct PosColTexVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 TexCoord;
};
struct PosRecColVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Rectangle;
	DirectX::XMFLOAT4 Color;
};
