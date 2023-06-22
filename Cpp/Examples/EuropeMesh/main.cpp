// Smoothing example from Section 7.1.1 of the GETMe book.
#include "Common/reporting.h"
#include "Common/smoothing_headers.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
  const auto dataPath = std::filesystem::path(argv[0]).parent_path();

  std::cout << "\nThis program demonstrates smoothing of a not simply\n"
               "connected distorted mixed mesh of parts of Europe.\n"
               "Cf. Section 7.1.1 of the book Vartziotis, Wipper:\n"
               "The GETMe Mesh Smoothing Framework, CRC Press, 2018.";

  // Read inital mesh and report quality.
  const auto fileName = "europe_mixed_initial.mesh";
  const auto meshFilePath = dataPath / fileName;
  const auto initialMesh = Mesh::readMeshFile(meshFilePath);
  std::cout << "\n\nExample: " << fileName << "\n";
  Common::printInitialMeshInformation(initialMesh);

  // Set algorithm parameters.
  const Smoothing::SmartLaplaceConfig smartLaplaceConfig;
  const Smoothing::GetmeConfig getmeConfig(
      initialMesh.getMaximalNumberOfPolygonNodes());

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

  std::cout << "\nResults differ slightly from those given in the GETMe book, "
               "since an improved min heap sorting scheme is used in this "
               "implementation.\n\nResults can be visualized by using the "
               "Matlab function showallmeshes.\n\n";

  return 0;
}
