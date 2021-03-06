float3 getSkySHDiffuse(float3 normal)
{
	float4 normalVector = float4(normal, 1);
	float3 intermediate0, intermediate1, intermediate2;
	intermediate0.x = dot(View.SkyIrradianceEnvironmentMap[0], normalVector);
	intermediate0.y = dot(View.SkyIrradianceEnvironmentMap[1], normalVector);
	intermediate0.z = dot(View.SkyIrradianceEnvironmentMap[2], normalVector);
	
	float4 vb = normalVector.xyzz * normalVector.yzzx;
	intermediate1.x = dot(View.SkyIrradianceEnvironmentMap[3], vb);
	intermediate1.y = dot(View.SkyIrradianceEnvironmentMap[4], vb);
	intermediate1.z = dot(View.SkyIrradianceEnvironmentMap[5], vb);

	float vc = normalVector.x * normalVector.x - normalVector.y * normalVector.y;

	intermediate2 = View.SkyIrradianceEnvironmentMap[6].xyz * vc;

	return max(0, intermediate0 + intermediate1 + intermediate2);
}

#define REFLECTION_CAPTURE_ROUGHEST_MIP 1
#define REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE 1.2

float computeReflectionCaptureRoughnessFromMip(float mip, half cubemapMaxMip)
{
	float levelFrom1x1 = cubemapMaxMip - 1 - mip;
	return exp2((REFLECTION_CAPTURE_ROUGHEST_MIP - levelFrom1x1) / REFLECTION_CAPTURE_ROUGHNESS_MIP_SCALE);
}