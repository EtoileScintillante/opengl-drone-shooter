/*                                  
 * vertex_data.h
 *
 * Created by: EtoileScintillante
 * This file contains all the vertices needed for rendering the blocks.
 */

#ifndef __VERTEX_DATA_H__
#define __VERTEX_DATA_H__

#include <vector>

std::vector < float > getPositionData()
{
    std::vector v = {
    // positions
    -0.5f, -0.5f, -0.5f,    
     0.5f, -0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
    -0.5f,  0.5f, -0.5f,   
    -0.5f, -0.5f, -0.5f,   

    -0.5f, -0.5f,  0.5f,    
     0.5f, -0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f,  0.5f,   
    -0.5f, -0.5f,  0.5f,   

    -0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

     0.5f,  0.5f,  0.5f,    
     0.5f,  0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   

    -0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f, -0.5f,   
     0.5f, -0.5f,  0.5f,   
     0.5f, -0.5f,  0.5f,   
    -0.5f, -0.5f,  0.5f,   
    -0.5f, -0.5f, -0.5f,   

    -0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f, -0.5f,   
     0.5f,  0.5f,  0.5f,   
     0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f,  0.5f,   
    -0.5f,  0.5f, -0.5f
    };

    return v;
};

std::vector < float > getTextureCoordsData()
{
    std::vector< float > v = {
     // trunk       // dirt        // leaves/stone  // creeper/zombie
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.0f, 0.5f, // side    
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,      0.5f, 0.5f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      0.5f, 1.0f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      0.5f, 1.0f,
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,      0.0f, 1.0f,
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.0f, 0.5f,

     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.5f, 0.5f, // side
     1.0f, 0.5f,    0.5f, 0.75f,   1.0f, 0.0f,      1.0f, 0.5f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.72f,   0.5f, 1.0f,    1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.72f,   0.0f, 1.0f,    0.0f, 1.0f,      0.5f, 1.0f,
     0.5f, 0.5f,    0.0f, 0.75f,   0.0f, 0.0f,      0.5f, 0.5f,

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f, // side
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,      0.5f, 0.5f,
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f,

     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f, // side 
     1.0f, 0.72f,   0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     1.0f, 0.5f,    0.5f, 1.0f,    0.0f, 1.0f,      1.0f, 0.5f,
     0.5f, 0.5f,    0.0f, 1.0f,    0.0f, 0.0f,      0.5f, 0.5f,
     0.5f, 0.72f,   0.0f, 0.75f,   1.0f, 0.0f,      0.5f, 1.0f,

     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,      0.5f, 1.0f, // bottom
     0.5f, 0.5f,    0.5f, 0.75f,   1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.5f, 0.745f,  0.5f, 0.90f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.0f, 0.745f,  0.0f, 0.90f,   0.0f, 0.0f,      0.5f, 0.5f,
     0.0f, 0.5f,    0.0f, 0.75f,   0.0f, 1.0f,      0.5f, 1.0f,

     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f,      0.5f, 1.0f, // top
     0.5f, 0.5f,    1.0f,  0.75f,  1.0f, 1.0f,      1.0f, 1.0f,
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.5f, 0.745f,  1.0f,  1.0f,   1.0f, 0.0f,      1.0f, 0.5f,
     0.0f, 0.745f,  0.52f, 1.0f,   0.0f, 0.0f,      0.5f, 0.5f,
     0.0f, 0.5f,    0.52f, 0.75f,  0.0f, 1.0f,      0.5f, 1.0f
    };

    return v;
}

std::vector< float > getSkyboxPositionData()
{
    std::vector< float > v = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    return v;
};

#endif /*__VERTEX__DATA__*/