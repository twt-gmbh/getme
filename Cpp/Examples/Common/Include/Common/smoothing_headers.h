/*
Convenience header including smoothing relevant headers.

Copyright (C) 2023  TWT GmbH Science & Innovation.

This file is part of GETMe Polygonal Meshes 2D.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
#pragma once

#include "Mathematics/bounding_box.h"
#include "Mathematics/polygon_algorithms.h"
#include "Mathematics/vector2d_algorithms.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/basic_getme_simultaneous_config.h"
#include "Smoothing/basic_laplace_config.h"
#include "Smoothing/default_configuration.h"
#include "Smoothing/getme_algorithms.h"
#include "Smoothing/getme_config.h"
#include "Smoothing/getme_result.h"
#include "Smoothing/getme_sequential_config.h"
#include "Smoothing/getme_simultaneous_config.h"
#include "Smoothing/laplace_algorithms.h"
#include "Smoothing/smart_laplace_config.h"
#include "Smoothing/smoothing_result.h"
