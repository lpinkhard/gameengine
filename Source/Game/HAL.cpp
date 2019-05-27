//
//  HAL.cpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/27.
//  Copyright © 2019 Lionel Pinkhard. All rights reserved.
//

#include "HAL.hpp"

#ifdef DX_UWP
#include "../Platforms/UWP/Common/DirectXHelper.h"

using namespace DirectX;
using namespace Windows::Foundation;
#endif

HAL::HAL() {
	lastSurface = NULL;
}

void HAL::reshape(int width, int height, int scale) {
	// Save dimensions
	this->width = width;
	this->height = height;
	this->scale = scale;

#ifndef GLMODE_NONE
    // Set OpenGL viewport
    glViewport(0, 0, width, height);
        
    // Defualt orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
#ifdef GLMODE_ES
    glOrthof(0.0f, width, height, 0.0f, -1.0f, 1.0f);
#else
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
#endif
    
    // Prepare to draw
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

#ifdef DX_UWP
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
#endif
}

bool HAL::init(InputEvent *inputEvent) {
#ifndef GLMODE_NONE
	// Clear to color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // Smooth points
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
#endif

#ifdef DX_UWP
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"BasicVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"BasicPixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
			)
		);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
			)
		);
	});
#endif

    this->inputEvent = inputEvent;
    
    lastPointerX = width / 2;
    lastPointerY = height / 2;
    
    return true;
}

void HAL::putPixel(GfxSurface *surface, int x, int y, int r, int g, int b, int a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x), static_cast<GLshort>(y), 0
    };
    
    // Draw pixel
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_POINTS, 0, 1);
#endif

#ifdef DX_UWP
#endif
}

int HAL::getWidth() {
    return width;
}

int HAL::getHeight() {
    return height;
}

void HAL::line(GfxSurface *surface, int x1, int y1, int x2, int y2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x1), static_cast<GLshort>(y1), 0,
        static_cast<GLshort>(x2), static_cast<GLshort>(y2), 0
    };
    
    // Draw line
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_LINES, 0, 2);
#endif

#ifdef DX_UWP
#endif
}

void HAL::hline(GfxSurface *surface, int x, int y1, int y2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x), static_cast<GLshort>(y1), 0,
        static_cast<GLshort>(x), static_cast<GLshort>(y2), 0
    };
    
    // Draw line
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_LINES, 0, 2);
#endif

#ifdef DX_UWP
#endif
}

void HAL::vline(GfxSurface *surface, int x1, int y, int x2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x1), static_cast<GLshort>(y), 0,
        static_cast<GLshort>(x2), static_cast<GLshort>(y), 0
    };
    
    // Draw line
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_LINES, 0, 2);
#endif

#ifdef DX_UWP
#endif
}

void HAL::circle(GfxSurface *surface, int x, int y, int radius, int r, int g, int b, int a) {
    changeSurface(surface);

    // Multiplication factor
    int mFactor = 8;
    
    // Find angular change over four times radius
    float angle = 2 * 3.141592653589793238 / float(radius * mFactor);

    // Calculate tangent and cosine
    float tanAngle = tanf(angle);
    float cosAngle = cosf(angle);
    
    // Start at angle of 0
    float dx = radius;
    
    // Start y at origin
    float dy = 0;
    
#ifndef GLMODE_NONE
    GLshort *vertexData = new GLshort[3 * radius * mFactor];
    
    for (int i = 0; i < radius * mFactor; i++)
    {
        // Line point around circle
        vertexData[i * 3] = dx + x;
        vertexData[i * 3 + 1] = dy + y;
        vertexData[i * 3 + 2] = 0;
        
        // Calculate vector to next point
        float tx = -dy;
        float ty = dx;
        
        // Adjust by vector according to slope
        dx += tx * tanAngle;
        dy += ty * tanAngle;
        
        // Adjust for cosine of angle
        dx *= cosAngle;
        dy *= cosAngle;
    }

    // Draw circle
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, vertexData);
    glDrawArrays(GL_LINE_LOOP, 0, radius * mFactor);
    
    delete[] vertexData;
#endif

#ifdef DX_UWP
#endif
}

void HAL::circleFill(GfxSurface *surface, int x, int y, int radius, int r, int g, int b, int a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x), static_cast<GLshort>(y), 0
    };
    
    // Draw filled circle
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glPointSize(radius * 2);
    glDrawArrays(GL_POINTS, 0, 1);

    glPointSize(1);
#endif

#ifdef DX_UWP
#endif
}

void HAL::rect(GfxSurface *surface, int x1, int y1, int x2, int y2,
               uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    changeSurface(surface);

#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x1), static_cast<GLshort>(y1), 0,
        static_cast<GLshort>(x2), static_cast<GLshort>(y1), 0,
        static_cast<GLshort>(x2), static_cast<GLshort>(y2), 0,
        static_cast<GLshort>(x1), static_cast<GLshort>(y2), 0
    };
    
    // Draw rectangle
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
#endif

#ifdef DX_UWP
#endif
}

void HAL::rectFill(GfxSurface *surface, int x1, int y1, int x2, int y2,
                   uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    triangleFill(surface, x1, y1, x2, y1, x1, y2, r, g, b, a);
    triangleFill(surface, x1, y2, x2, y1, x2, y2, r, g, b, a);
}

void HAL::triangleFill(GfxSurface *surface, int x1, int y1, int x2, int y2, int x3, int y3,
                   uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    changeSurface(surface);
    
#ifndef GLMODE_NONE
    GLshort vertexData[] = {
        static_cast<GLshort>(x1), static_cast<GLshort>(y1), 0,
        static_cast<GLshort>(x2), static_cast<GLshort>(y2), 0,
        static_cast<GLshort>(x3), static_cast<GLshort>(y3), 0,
    };
    
    // Draw triangle
    glColor4ub(r, g, b, a);
    glVertexPointer(3, GL_SHORT, 0, &vertexData);
    glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

#ifdef DX_UWP
#endif
}

void HAL::flip(GfxSurface *surface) {
#ifndef GLMODE_NONE
	glFlush();
#endif

#ifdef DX_UWP
#endif
}

void HAL::shutdown() {
#ifdef DX_UWP
	resetDDR();
#endif
}

#ifdef DX_UWP
void HAL::resetDDR() {
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}
#endif

void HAL::changeSurface(GfxSurface *surface) {
    if (surface != lastSurface) {
        if (surface != NULL) {
            // Set custom surface
        } else {
            // Back to default surface
        }
        lastSurface = surface;
    }
}

#ifdef DX_UWP
void HAL::setDX(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	m_deviceResources = deviceResources;
}
#endif

void HAL::eventPointerClick(int button, int x, int y) {
    inputEvent->pointerClick(button, x, y);
}

void HAL::eventPointerRelease(int button, int x, int y) {
    inputEvent->pointerRelease(button, x, y);
}

void HAL::eventPointerMove(int x, int y) {
    int dx = x - lastPointerX;
    int dy = y - lastPointerY;
    
    inputEvent->pointerMove(x, y, dx / (width / 2), dy / (width / 2));
    lastPointerX = x;
    lastPointerY = y;
}

void HAL::eventPointerClick(int button) {
    inputEvent->pointerClick(button, lastPointerX, lastPointerY);
}

void HAL::eventPointerRelease(int button) {
    inputEvent->pointerRelease(button, lastPointerX, lastPointerY);
}

void HAL::eventKeyUp(int key) {
    inputEvent->keyPress(key);
}

void HAL::eventKeyDown(int key) {
    inputEvent->keyRelease(key);
}
