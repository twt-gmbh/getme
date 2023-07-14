/*
Smoothing example from Section 7.4.2 of the GETMe book.

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
#include "Common/reporting.h"
#include "Common/smoothing_headers.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
  const auto dataPath = std::filesystem::path(argv[0]).parent_path();

  std::cout << "\nThis program demonstrates smoothing of distorted single\n"
               "element type finite element meshes of an involute gear.\n"
               "Cf. Section 7.4.2 of the book Vartziotis, Wipper: The GETMe\n"
               "Mesh Smoothing Framework, CRC Press, 2018.\n";

  // Set algorithm parameters.
  const Smoothing::SmartLaplaceConfig smartLaplaceConfig;
  const std::size_t maxNumberOfPolygonNodes = 4;
  const Smoothing::GetmeConfig getmeConfig(maxNumberOfPolygonNodes);

  // Loop over mesh types.
  for (const auto fileName :
       {"gear_tri_initial.mesh", "gear_quad_initial.mesh"}) {
    const auto meshFilePath = dataPath / fileName;
    const auto initialMesh = Mesh::readMeshFile(meshFilePath);
    std::cout << "\nExample: " << fileName << "\n";
    Common::printInitialMeshInformation(initialMesh);

    // Smooth mesh using smart Laplace.
    const auto smartLaplaceResult =
        Smoothing::smartLaplace(initialMesh, smartLaplaceConfig);
    Common::printSmoothingResult(smartLaplaceResult);
    Common::writeResultMesh(smartLaplaceResult.mesh, meshFilePath,
                            "smart_laplace");

    // Smooth mesh using GETMe.
    const auto getmeResult = Smoothing::getme(initialMesh, getmeConfig);
    Common::printSmoothingResult(getmeResult);
    Common::writeResultMesh(getmeResult.mesh, meshFilePath, "getme");
  }

  std::cout << "\nResults can be visualized by using the Matlab function "
               "showallmeshes.\n\n";

  return 0;
}
