/*                                  
 * vertex_data.h
 *
 * Created by: EtoileScintillante
 * This file contains all the vertices needed for rendering the blocks.
 */

#ifndef __VERTEX_DATA_H__
#define __VERTEX_DATA_H__

#include <vector>

std::vector < float > getPositionData();

std::vector < float > getTextureCoordsData();

std::vector< float > getSkyboxPositionData();

#endif /*__VERTEX__DATA__*/