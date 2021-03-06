SET(LIB_NAME Renderer)

SET(SOURCE_FILE
    ${Renderer_SOURCE_DIR}/RendererInterface.h 
    ${Renderer_SOURCE_DIR}/RendererMininal.h 
    ${Renderer_SOURCE_DIR}/RendererModule.h 
    ${Renderer_SOURCE_DIR}/RendererModule.cpp 
    ${Renderer_SOURCE_DIR}/Renderer.h 
    ${Renderer_SOURCE_DIR}/Renderer.cpp 
    ${Renderer_SOURCE_DIR}/scene.h 
    ${Renderer_SOURCE_DIR}/scene.cpp 
    ${Renderer_SOURCE_DIR}/sceneRendering.h 
    ${Renderer_SOURCE_DIR}/sceneRendering.cpp 
    ${Renderer_SOURCE_DIR}/DeferredShadingRenderer.h 
    ${Renderer_SOURCE_DIR}/DeferredShadingRenderer.cpp 
    ${Renderer_SOURCE_DIR}/DistanceFieldSurfaceCacheLighting.h 
    ${Renderer_SOURCE_DIR}/DistanceFieldSurfaceCacheLighting.cpp 
    ${Renderer_SOURCE_DIR}/BasePassRendering.h 
    ${Renderer_SOURCE_DIR}/GlobalDistanceField.h 
    ${Renderer_SOURCE_DIR}/GlobalDistanceField.cpp 
    ${Renderer_SOURCE_DIR}/BasePassRendering.cpp 
    ${Renderer_SOURCE_DIR}/RendererScene.cpp 
    ${Renderer_SOURCE_DIR}/ScenePrivate.h 
    ${Renderer_SOURCE_DIR}/ScenePrivate.cpp 
    ${Renderer_SOURCE_DIR}/SystemTextures.h 
    ${Renderer_SOURCE_DIR}/SystemTextures.cpp 
    ${Renderer_SOURCE_DIR}/DepthRendering.h 
    ${Renderer_SOURCE_DIR}/DepthRendering.cpp 
    ${Renderer_SOURCE_DIR}/SceneVisibility.cpp 
    ${Renderer_SOURCE_DIR}/LightSceneInfo.h 
    ${Renderer_SOURCE_DIR}/LightSceneInfo.cpp 
    ${Renderer_SOURCE_DIR}/SceneCore.h 
    ${Renderer_SOURCE_DIR}/SceneCore.cpp 
    ${Renderer_SOURCE_DIR}/PrimitiveSceneInfo.h 
    ${Renderer_SOURCE_DIR}/PrimitiveSceneInfo.cpp 
    ${Renderer_SOURCE_DIR}/LightMapRendering.h 
    ${Renderer_SOURCE_DIR}/LightMapRendering.cpp 
    ${Renderer_SOURCE_DIR}/ScenePrivateBase.h 
    ${Renderer_SOURCE_DIR}/ScenePrivateBase.cpp 
    ${Renderer_SOURCE_DIR}/ShaderBaseClasses.h 
    ${Renderer_SOURCE_DIR}/ShaderBaseClasses.cpp 
    ${Renderer_SOURCE_DIR}/LightRendering.h 
    ${Renderer_SOURCE_DIR}/LightRendering.cpp 
    ${Renderer_SOURCE_DIR}/LightFunctionRendering.cpp 
    ${Renderer_SOURCE_DIR}/TiledDeferredLightRendering.cpp 
    ${Renderer_SOURCE_DIR}/ShadowRendering.h 
    ${Renderer_SOURCE_DIR}/ShadowRendering.cpp 
    ${Renderer_SOURCE_DIR}/HdrCustomResolveShaders.h 
    ${Renderer_SOURCE_DIR}/HdrCustomResolveShaders.cpp 
    ${Renderer_SOURCE_DIR}/SkyBoxRendering.cpp 
    ${Renderer_SOURCE_DIR}/ReflectionEnvironmentCapture.h 
    ${Renderer_SOURCE_DIR}/ReflectionEnvironmentCapture.cpp 
    ${Renderer_SOURCE_DIR}/ReflectionEnvironmentDiffuseIrradiance.cpp 
    ${Renderer_SOURCE_DIR}/MeshPassProcessor.h 
    ${Renderer_SOURCE_DIR}/MeshPassProcessor.cpp 
    ${Renderer_SOURCE_DIR}/MeshDrawShaderBindings.h 
    ${Renderer_SOURCE_DIR}/MeshDrawCommands.h 
    ${Renderer_SOURCE_DIR}/MeshDrawCommands.cpp 
    ${Renderer_SOURCE_DIR}/GPUScene.h 
    ${Renderer_SOURCE_DIR}/GPUScene.cpp 
    ${Renderer_SOURCE_DIR}/ByteBuffer.h
    ${Renderer_SOURCE_DIR}/ByteBuffer.cpp
    ${Renderer_SOURCE_DIR}/ScreenPass.h
    ${Renderer_SOURCE_DIR}/ScreenPass.cpp
    ${Renderer_SOURCE_DIR}/DistanceFieldAmbientOcclusion.cpp
    ${Renderer_SOURCE_DIR}/SceneCaptureRendering.cpp
    ${Renderer_SOURCE_DIR}/LightGridInjection.cpp
    ${Renderer_SOURCE_DIR}/VolumetricFog.cpp
    ${Renderer_SOURCE_DIR}/ClusteredDeferredShadingPass.cpp
    ${Renderer_SOURCE_DIR}/TranslucentLighting.cpp
    ${Renderer_SOURCE_DIR}/VolumeRendering.h
    ${Renderer_SOURCE_DIR}/VolumeRendering.cpp
    ${Renderer_SOURCE_DIR}/IndirectLightRendering.cpp
    ${Renderer_SOURCE_DIR}/CapsuleShadowRendering.cpp
    )
    

SET(PostProcess_Source
    ${Renderer_SOURCE_DIR}/PostProcess/SceneRenderTargets.h 
    ${Renderer_SOURCE_DIR}/PostProcess/SceneRenderTargets.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/RenderTargetPool.h 
    ${Renderer_SOURCE_DIR}/PostProcess/RenderTargetPool.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/SceneFilterRendering.h 
    ${Renderer_SOURCE_DIR}/PostProcess/SceneFilterRendering.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessing.h 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessing.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/RenderingCompositionGraph.h 
    ${Renderer_SOURCE_DIR}/PostProcess/RenderingCompositionGraph.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessParameters.h 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessParameters.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessTonemap.h 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessTonemap.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessInput.h 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessInput.cpp 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessCombineLUTs.h 
    ${Renderer_SOURCE_DIR}/PostProcess/PostProcessCombineLUTs.cpp 
    )
SOURCE_GROUP(PostProcess FILES ${PostProcess_Source})


SET(RayTracing_Source
    ${Renderer_SOURCE_DIR}/RayTracing/RaytracingOptions.h 
	)

SOURCE_GROUP(RayTracing FILES ${RayTracing_Source})


SET(CompositionLighting_Source
	${Renderer_SOURCE_DIR}/CompositionLighting/CompositionLighting.h
	${Renderer_SOURCE_DIR}/CompositionLighting/CompositionLighting.cpp
	${Renderer_SOURCE_DIR}/CompositionLighting/PostProcessAmbient.h
	${Renderer_SOURCE_DIR}/CompositionLighting/PostProcessAmbient.cpp
)
SOURCE_GROUP(CompositionLighting, FILES ${CompositionLighting_Source})

ADD_LIBRARY(${LIB_NAME} ${AIR_PREFERRED_LIB_TYPE} 
    ${SOURCE_FILE}
    ${PostProcess_Source}
    ${CompositionLighting_Source}
    ${RayTracing_Source}
)
SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES FOLDER Engine/Runtime)

target_link_libraries(${LIB_NAME}  
    core
    engine
    RenderCore
    RHI
	UtilityShader
)


INCLUDE(${AIR_CMAKE_MODULE_DIR}/TargetsCommonSetting.cmake)