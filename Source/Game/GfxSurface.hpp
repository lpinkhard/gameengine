//
//  GfxSurface.hpp
//  GameEngine
//
//  Created by Lionel Pinkhard on 2019/01/28.
//  Copyright © 2019 Lionel Pinkhard. All rights reserved.
//

#ifndef GfxSurface_hpp
#define GfxSurface_hpp

#include <stdio.h>

class GfxSurface {
public:
    void *getSurface();
    
private:
    void *sdlSurface;
};

#endif /* GfxSurface_hpp */
