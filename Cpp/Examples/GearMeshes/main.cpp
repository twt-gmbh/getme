// Smoothing example from Section 7.4.2 of the GETMe book.
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
