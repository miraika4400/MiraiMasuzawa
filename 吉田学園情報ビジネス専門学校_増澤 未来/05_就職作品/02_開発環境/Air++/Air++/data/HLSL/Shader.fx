float4x4 WorldViewProj : WORLDVIEWPROJ;
float4x4 World :WORLD;
float3   LightDirection;
float4   DiffuseColor;
float3   Eye;
float4   SpecularColor;

struct VS_OUTPUT {
	float4 Posision :POSITION;      // 座標
	float2 TexCoord :TEXCOORD;      // テクスチャ座標
	float4 Color    :COLOR0;         // カラー
	float4 Specular :COLOR1;        // スペキュラー
	float3 CubeTexCoord :TEXCOORD1; // キューブテクスチャ
};

struct VS_OUTPUT2 {
	float4 Posision :POSITION; // 座標
	float4 Color    :COLOR0;    // カラー
	float4 Specular :COLOR1;   // スペキュラー
};


texture Tex;
sampler Sampler = sampler_state {
	Texture = Tex;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture CubeTex;
samplerCUBE SamplerCube = sampler_state {
	Texture = CubeTex;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// テクスチャありピクセルシェーダ
float4 PsTex(VS_OUTPUT In) :COLOR
{
	float4 fOut = tex2D(Sampler, In.TexCoord) * (In.Color + In.Specular);

	return fOut;
}

// テクスチャなしピクセル
float4 PsNotTex(VS_OUTPUT In) :COLOR
{
	float4 fOut = In.Color + In.Specular;
	fOut.a = 1;

	return texCUBE(SamplerCube, In.CubeTexCoord)/4 + fOut;
}


VS_OUTPUT VS(float3 Position : POSITION, float2 TexCoord : TEXCOORD,float4 Diffuse : COLOR0, float3 Normal : NORMAL)
{
	VS_OUTPUT Out;
	Out.Posision = mul(float4(Position, 1), WorldViewProj);
	Out.Color = DiffuseColor;
	
	//Out.TexCoord = TexCoord;
	float3 N = mul(Normal, (float3x3)World);
	N = normalize(N);
	float3 E = mul(Eye, (float3x3)World);
	E = normalize(E);
	float3 H = normalize(normalize(LightDirection) + normalize(Eye - Position));

	Out.Specular = SpecularColor * dot(N, H);
	Out.Specular = pow(Out.Specular, 8);
	Out.Specular.a = 1.0f;
	float LightPower = dot(N, LightDirection);
	LightPower = max(0, LightPower);
	Out.Color = DiffuseColor * LightPower;
	Out.Color.a = 1.0f;

	Out.TexCoord = TexCoord;

	Out.CubeTexCoord = reflect(Out.Posision - E, N);

	return Out;
}

float4 VS2(float3 Position : POSITION) :POSITION
{

	float4 Out;
	Out = mul(float4(Position*1.04, 1), WorldViewProj);

	return Out;
}

float4 PS2(VS_OUTPUT In) :COLOR
{

	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}

technique TexShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PsTex();
	}
	//pass P1
	//{
	//	VertexShader = compile vs_3_0 VS2();
	//	PixelShader = compile ps_3_0 PS2();
	//}
}

technique NotTexShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PsNotTex();
	}
	//pass P1
	//{
	//	VertexShader = compile vs_3_0 VS2();
	//	PixelShader = compile ps_3_0 PS2();
	//}
}