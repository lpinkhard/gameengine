//
//  HAL.hpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/27.
//  Copyright © 2019 Lionel Pinkhard. All rights reserved.
//

#ifndef HAL_hpp
#define HAL_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cmath>

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <OpenGLES/ES1/gl.h>
        #define GLMODE_ES 1
    #else
        #include <OpenGL/gl.h>
        #define GLMODE_NONES 1
    #endif
#else
	#ifdef _WIN32
		#include "../Platforms/UWP/pch.h"
		#include "../Platforms/UWP/Common/DeviceResources.h"
		#include "../Platforms/UWP/Content/ShaderStructures.h"
		#define GLMODE_NONE 1
		#define DX_UWP 1
	#else
		#include <OpenGL/gl.h>
		#define GLMODE_NONES 1
	#endif
#endif

#include "GfxSurface.hpp"
#include "InputEvent.hpp"

class HAL {
public:
    HAL();

    // Init and shutdown
    bool init(InputEvent *inputEvent);
	void reshape(int width, int height, int scale);
    void shutdown();
    
    // Screen details
    int getWidth();
    int getHeight();

    // Graphics primitives
    void putPixel(GfxSurface *surface, int x, int y, int r, int g, int b, int a);
    void line(GfxSurface *surface, int x1, int y1, int x2, int y2,
              uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void hline(GfxSurface *surface, int x1, int y, int x2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void vline(GfxSurface *surface, int x, int y1, int y2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void circle(GfxSurface *surface, int x, int y, int radius, int r, int g, int b, int a);
    void circleFill(GfxSurface *surface, int x, int y, int radius, int r, int g, int b, int a);
    void rect(GfxSurface *surface, int x1, int y1, int x2, int y2,
              uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void rectFill(GfxSurface *surface, int x1, int y1, int x2, int y2,
                  uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void triangleFill(GfxSurface *surface, int x1, int y1, int x2, int y2, int x3, int y3,
                  uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    // Event receivers
    void eventPointerMove(int x, int y);
    void eventPointerClick(int button);
    void eventPointerRelease(int button);
    void eventPointerClick(int button, int x, int y);
    void eventPointerRelease(int button, int x, int y);
    void eventKeyDown(int key);
    void eventKeyUp(int key);
    
    // Graphics buffering
    void flip(GfxSurface *surface);

#ifdef DX_UWP
	void setDX(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	void resetDDR();
#endif

private:
    void changeSurface(GfxSurface *surface);
    
#ifdef DX_UWP
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;
#endif

    GfxSurface *lastSurface;
    int width;
    int height;
    int scale;
    
    int lastPointerX, lastPointerY;
    
    InputEvent *inputEvent;
};

#endif /* HAL_hpp */
