struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
	float4 Reflection;
	float4 Transparent;
};

void ComputeDirectionalLight(DirectionalLight DirLight, Material material, float3 NormalW, float3 ToCameraW, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
	Diffuse = float4(0.f, 0.f, 0.f, 0.f);
	Specular = float4(0.f, 0.f, 0.f, 0.f);

	Ambient = material.Ambient * DirLight.Ambient;

	float3 lightVector = -DirLight.Direction;
		float diffuseFactor = dot(lightVector, NormalW);

	[flatten]
	if (diffuseFactor > 0.f)
	{
		float3 v = reflect(-lightVector, NormalW);
			float specularFactor = pow(max(dot(v, ToCameraW), 0.f), material.Specular.w);
		Diffuse = diffuseFactor * material.Diffuse * DirLight.Diffuse;
		Specular = specularFactor * material.Specular * DirLight.Specular;
	}
}
float3 DecompressNormalVector(float3 CompressVector, float3 UnitNormalW, float4 TangentW)
{
	float3 normalT = 2.f * CompressVector - 1.f;
	normalT.z = sqrt(1.f - saturate(dot(normalT.xy, normalT.xy)));

		float3 T = normalize(TangentW.xyz - dot(TangentW.xyz, UnitNormalW) * UnitNormalW);
		float3 B = cross(UnitNormalW, T) * TangentW.w;

		return mul(normalT, float3x3(T, B, UnitNormalW));
}