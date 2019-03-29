{
	"targets":[
		{
			"target_name": "alltargets",
			"type": "none",
			'variables': {
				'shaderPath': '../Shaders',
				'materialPath': '../assets/materials',
				'gypPath': '../gyp',
			},
			"include_dirs":[
				"<(shaderPath)",
			],
			"dependencies":[
				"core",
				"engine",
				"launch",
				"RHI",
				"RenderCore",
				"movePlayer",
				"WindowsTargetPlatformModule",
				"slateCore",
				"slate",
				#"slateNullRenderer",
				"slateRHIRenderer",
				"D3D11RHI",
				"HeadMountedDisplay",
				"Renderer",
				"EngineSetting",
				"CoreObject",
				"MovieSceneCapture",
				"ShaderCore",
				"Demo",
				"UtilityShader",
				"TargetPlatform",
				"DerivedDataCache",
				"ShaderFormatD3D",
				"ShaderCompilerCommon",
				"ShaderPreprocessor",
				"InputCore",
                "AssetsImporter",
                "FbxFactory",
                "EditorEngine",
				"Test",
			],
			"sources":[	
				"<(shaderPath)/BasePassPixelShader.hlsl",
				"<(shaderPath)/BasePassVertexCommon.hlsl",
				"<(shaderPath)/BasePassVertexShader.hlsl",
				"<(shaderPath)/Common.hlsl",
				"<(shaderPath)/LocalVertexFactory.hlsl",
				"<(shaderPath)/LocalVertexFactoryCommon.hlsl",
				"<(shaderPath)/MaterialTemplate.hlsl",
				"<(shaderPath)/PixelShaderOutputCommon.hlsl",
				"<(shaderPath)/VertexFactoryCommon.hlsl",
				"<(shaderPath)/BasePassCommon.hlsl",
				"<(shaderPath)/InstancedStereo.hlsl",
				"<(shaderPath)/Definitions.hlsl",
				"<(shaderPath)/DeferredShadingCommon.hlsl",
				"<(shaderPath)/ResolvePixelShader.hlsl",
				"<(shaderPath)/ResolveVertexShader.hlsl",
				"<(shaderPath)/OneColorShader.hlsl",
				"<(shaderPath)/DeferredLightPixelShaders.hlsl",
				"<(shaderPath)/DeferredLightVertexShaders.hlsl",
				"<(shaderPath)/DeferredLightingCommon.hlsl",
				"<(shaderPath)/Random.hlsl",
				"<(shaderPath)/LightAccumulator.hlsl",
				"<(shaderPath)/ShadingModels.hlsl",
				"<(shaderPath)/BRDF.hlsl",
				"<(shaderPath)/PostProcessTonemap.hlsl",
				"<(shaderPath)/HdrCustomResolveShaders.hlsl",
				"<(shaderPath)/PostProcessCommon.hlsl",
				"<(shaderPath)/TonemapCommon.hlsl",
				"<(shaderPath)/SkyBox.hlsl",
                
				"<(materialPath)/defaulLightFunctionMaterial.mtl",
				"<(materialPath)/defaultDeferredDecalMaterial.mtl",
				"<(materialPath)/defaultMaterial.mtl",
				"<(materialPath)/defaultPostProcessMaterial.mtl",

				#"<(gypPath)/Air2.gyp",
				"<(gypPath)/common_include.gypi",
				"<(gypPath)/core.gypi",
				"<(gypPath)/CoreObject.gypi",
				"<(gypPath)/D3D11RHI.gypi",
				"<(gypPath)/Demo.gypi",
				"<(gypPath)/DerivedDataCache.gypi",
				"<(gypPath)/engine.gypi",
				"<(gypPath)/engineSetting.gypi",
				"<(gypPath)/HeadMountedDisplay.gypi",
				"<(gypPath)/launch.gypi",
				"<(gypPath)/movePlayer.gypi",
				"<(gypPath)/InputCore.gypi",
				"<(gypPath)/MovieSceneCapture.gypi",
				"<(gypPath)/renderCore.gypi",
				"<(gypPath)/Renderer.gypi",
				"<(gypPath)/RHI.gypi",
				"<(gypPath)/ShaderCompilerCommon.gypi",
				"<(gypPath)/ShaderFormatD3D.gypi",
				"<(gypPath)/ShaderPreprocessor.gypi",
				"<(gypPath)/slate.gypi",
				"<(gypPath)/slateCore.gypi",
				"<(gypPath)/SlateNullRenderer.gypi",
				"<(gypPath)/SlateRHIRenderer.gypi",
				"<(gypPath)/TargetPlatform.gypi",
				"<(gypPath)/Test.gypi",
				"<(gypPath)/UtilityShader.gypi",
				"<(gypPath)/AssetFactories/FbxFactory.gypi",
			],
		},
	],
	
	'includes': [
		"core.gypi",
		"engine.gypi",
		"launch.gypi",
		"RHI.gypi",
		"renderCore.gypi",
		"movePlayer.gypi",
		"WindowsTargetPlatformModule.gypi",
		"slateCore.gypi",
		"slate.gypi",
		#"SlateNullRenderer.gypi",
		"SlateRHIRenderer.gypi",
		"D3D11RHI.gypi",
		"HeadMountedDisplay.gypi",
		"Renderer.gypi",
		"engineSetting.gypi",
		"CoreObject.gypi",
		"MovieSceneCapture.gypi",
		"shaderCore.gypi",
		"Demo.gypi",
		"UtilityShader.gypi",
		"TargetPlatform.gypi",
		"DerivedDataCache.gypi",
		"ShaderFormatD3D.gypi",
		"ShaderCompilerCommon.gypi",
		"ShaderPreprocessor.gypi",
		"InputCore.gypi",
		"Test.gypi",
        "Editor.gypi",
        "AssetsImporter.gypi",
        "./AssetFactories/FbxFactory.gypi"
	],
	
}