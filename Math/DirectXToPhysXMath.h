#pragma once

inline physx::PxMat44 DirectXToPhysx(DirectX::CXMMATRIX m)
{
	return physx::PxMat44(
		physx::PxVec4(m.r[0].m128_f32[0], m.r[0].m128_f32[1], m.r[0].m128_f32[2], m.r[0].m128_f32[3]),
		physx::PxVec4(m.r[1].m128_f32[0], m.r[1].m128_f32[1], m.r[1].m128_f32[2], m.r[1].m128_f32[3]),
		physx::PxVec4(m.r[2].m128_f32[0], m.r[2].m128_f32[1], m.r[2].m128_f32[2], m.r[2].m128_f32[3]),
		physx::PxVec4(m.r[3].m128_f32[0], m.r[3].m128_f32[1], m.r[3].m128_f32[2], m.r[3].m128_f32[3])
		);
}
inline physx::PxMat44 DirectXToPhysx(DirectX::XMFLOAT4X4 &m)
{
	return physx::PxMat44(
		physx::PxVec4(m._11, m._12, m._13, m._14),
		physx::PxVec4(m._21, m._22, m._23, m._24),
		physx::PxVec4(m._31, m._32, m._33, m._34),
		physx::PxVec4(m._41, m._42, m._43, m._44)
		);
}