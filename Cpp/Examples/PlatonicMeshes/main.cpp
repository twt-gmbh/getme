/*
Smoothing of distorted Platonic meshes. Extended example compared to

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
// Section 6.1.1 of the GETMe book.
#include "Common/reporting.h"
#include "Common/smoothing_headers.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
  const auto dataPath = std::filesystem::path(argv[0]).parent_path();

  std::cout
      << "\nThis program demonstrates the usage of basic smoothing\n"
         "algorithms to untangle and smooth different distorted Platonic\n"
         "meshes. In addition, it shows the regularizing effect of basic\n"
         "GETMe simultaneous smoothing in contrast to basic Laplacian\n"
         "smoothing. Cf. Section 6.1.1 of the book Vartziotis, Wipper:\n"
         "The GETMe Mesh Smoothing Framework, CRC Press, 2018.\n";

  // Set algorithm parameters. For basic GETMe simultaneous use transformations
  // with lambda=1/2 and theta=pi/n, where n denotes the number of polygon
  // nodes.
  const double maxNodeRelocationDistanceThreshold = 0.01;
  const std::size_t maxNumberOfPolygonNodes = 12;
  const Smoothing::BasicLaplaceConfig basicLaplaceConfig(
      maxNodeRelocationDistanceThreshold);
  const Smoothing::BasicGetmeSimultaneousConfig basicGetmeSimultaneousConfig(
      maxNodeRelocationDistanceThreshold, maxNumberOfPolygonNodes,
      Smoothing::DefaultConfiguration::PolygonTransformationSet::Generic);

  // Loop over Platonic meshes.
  for (int meshNumber = 1; meshNumber <= 3; ++meshNumber) {
    // Load initial regular mesh and show information.
    const auto meshNumberString = std::to_string(meshNumber);
    const auto fileName = "platonic" + meshNumberString + "_initial.mesh";
    const auto meshFilePath = dataPath / fileName;
    const auto initialMesh = Mesh::readMeshFile(meshFilePath);

    std::cout << "\nExample: " << fileName << "\n";

    // Apply random distortion to regular mesh and show information.
    const double maxDistortionRadius = 5.0;
    const auto distortedMesh =
        Mesh::distortNodesLocally(initialMesh, maxDistortionRadius);
    Common::printInitialMeshInformation(distortedMesh);
    Common::writeResultMesh(distortedMesh, meshFilePath, "distorted");

    // Smooth mesh using basic Laplace.
    const auto basicLaplaceResult =
        Smoothing::basicLaplace(distortedMesh, basicLaplaceConfig);
    Common::printSmoothingResult(basicLaplaceResult);
    Common::writeResultMesh(basicLaplaceResult.mesh, meshFilePath,
                            "basic_laplace");

    // Smooth mesh using basic GETMe simultaneous.
    const auto basicGetmeSimultaneousResult = Smoothing::basicGetmeSimultaneous(
        distortedMesh, basicGetmeSimultaneousConfig);
    Common::printSmoothingResult(basicGetmeSimultaneousResult);
    Common::writeResultMesh(basicGetmeSimultaneousResult.mesh, meshFilePath,
                            "basic_getme_simultaneous");
  }
  std::cout << "\nResults can be visualized by using the Matlab function "
               "showallmeshes.\n\n";

  return 0;
}
