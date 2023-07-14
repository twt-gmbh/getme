/*
Algorithms for planar polygonal meshes.

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
#include "Mesh/polygonal_mesh_algorithms.h"

#include "Mathematics/polygon.h"
#include "Mathematics/polygon_algorithms.h"
#include "Mathematics/vector2d_algorithms.h"
#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <limits>

namespace {
const std::string PolygonalMeshKeyword = "planar_polygonal_mesh";
const std::string NodesKeyword = "nodes";
const std::string PolygonsKeyword = "polygons";
const std::string FixedNodeIndicesKeyword = "fixed_node_indices";
const std::string MeanRatioKeyword = "polygon_mean_ratio_quality_numbers";

void writeMeshHeader(std::ofstream& outfile) {
  outfile << PolygonalMeshKeyword << "\n";
}

void writeMeshNodes(const Mesh::PolygonalMesh& mesh, std::ofstream& outfile) {
  outfile << NodesKeyword << " " << mesh.getNumberOfNodes() << "\n";
  for (const auto& node : mesh.getNodes()) {
    outfile << node.getX() << " " << node.getY() << "\n";
  }
}

void writeMeshPolygons(const Mesh::PolygonalMesh& mesh,
                       std::ofstream& outfile) {
  outfile << PolygonsKeyword << " " << mesh.getNumberOfPolygons() << "\n";
  for (const auto& polygon : mesh.getPolygons()) {
    outfile << polygon.getNumberOfNodes();
    for (const auto nodeIndex : polygon.getNodeIndices()) {
      outfile << " " << nodeIndex;
    }
    outfile << "\n";
  }
}

void writeFixedNodeIndices(const Mesh::PolygonalMesh& mesh,
                           std::ofstream& outfile) {
  outfile << FixedNodeIndicesKeyword << " " << mesh.getFixedNodeIndices().size()
          << "\n";
  std::vector<std::size_t> sortedFixedNodeIndices(
      mesh.getFixedNodeIndices().begin(), mesh.getFixedNodeIndices().end());
  std::ranges::sort(sortedFixedNodeIndices);
  for (const auto nodeIndex : sortedFixedNodeIndices) {
    outfile << nodeIndex << "\n";
  }
}

void writeMeanRatioQualityNumbers(const Mesh::PolygonalMesh& mesh,
                                  std::ofstream& outfile) {
  const auto meanRatioQualityNumbers =
      Mesh::computeMeanRatioQualityNumberOfPolygons(mesh.getPolygons(),
                                                    mesh.getNodes());
  outfile << MeanRatioKeyword << " " << mesh.getNumberOfPolygons() << "\n";
  for (const auto qualityNumber : meanRatioQualityNumbers) {
    outfile << qualityNumber << "\n";
  }
}
}  // namespace

void Mesh::writeMeshFile(const PolygonalMesh& mesh,
                         const std::filesystem::path& outfilePath,
                         const bool includeMeanRatioQuality) {
  Utility::throwExceptionIfFalse(outfilePath.has_filename(),
                                 "No filename int outfile path given.");
  try {
    std::ofstream outfile(outfilePath);
    outfile.precision(std::numeric_limits<double>::max_digits10);
    outfile << std::scientific << std::showpos;
    writeMeshHeader(outfile);
    writeMeshNodes(mesh, outfile);
    writeMeshPolygons(mesh, outfile);
    writeFixedNodeIndices(mesh, outfile);
    if (includeMeanRatioQuality) {
      writeMeanRatioQualityNumbers(mesh, outfile);
    }
  } catch (const std::exception& e) {
    Utility::throwException(e.what());
  }
}

namespace {
void readPolygonalMeshHeader(std::ifstream& infile) {
  std::string line;
  std::getline(infile, line);
  const bool containsMeshKeyword =
      line.find(PolygonalMeshKeyword) != std::string::npos;
  Utility::throwExceptionIfFalse(containsMeshKeyword,
                                 "Mesh type information not found.");
}

std::vector<Mathematics::Vector2D> readMeshNodes(std::ifstream& infile) {
  std::string keyword;
  std::size_t numberOfNodes;
  infile >> keyword >> numberOfNodes;
  Utility::throwExceptionIfFalse(keyword == NodesKeyword,
                                 "Nodes keyword expected but not found.");
  std::vector<Mathematics::Vector2D> nodes;
  nodes.reserve(numberOfNodes);
  for (std::size_t nodeIndex = 0; nodeIndex < numberOfNodes; ++nodeIndex) {
    double x;
    double y;
    infile >> x >> y;
    nodes.emplace_back(x, y);
  }
  return nodes;
}

std::vector<Mathematics::Polygon> readMeshPolygons(std::ifstream& infile) {
  std::string keyword;
  std::size_t numberOfPolygons;
  infile >> keyword >> numberOfPolygons;
  Utility::throwExceptionIfFalse(keyword == PolygonsKeyword,
                                 "Polygons keyword expected but not found.");
  std::vector<Mathematics::Polygon> polygons;
  polygons.reserve(numberOfPolygons);
  for (std::size_t polygonIndex = 0; polygonIndex < numberOfPolygons;
       ++polygonIndex) {
    std::size_t numberOfNodeIndices;
    infile >> numberOfNodeIndices;
    std::vector<std::size_t> nodeIndices;
    nodeIndices.reserve(numberOfNodeIndices);
    for (std::size_t nodeNumber = 0; nodeNumber < numberOfNodeIndices;
         ++nodeNumber) {
      std::size_t nodeIndex;
      infile >> nodeIndex;
      nodeIndices.push_back(nodeIndex);
    }
    polygons.emplace_back(nodeIndices);
  }
  return polygons;
}

std::unordered_set<std::size_t> readFixedNodeIndices(std::ifstream& infile) {
  std::string keyword;
  std::size_t numberOfEntries;
  infile >> keyword >> numberOfEntries;
  Utility::throwExceptionIfFalse(
      keyword == FixedNodeIndicesKeyword,
      "Fixed node indices keyword expected but not found.");
  std::unordered_set<std::size_t> fixedNodeIndices;
  for (std::size_t index = 0; index < numberOfEntries; ++index) {
    std::size_t nodeIndex;
    infile >> nodeIndex;
    fixedNodeIndices.insert(nodeIndex);
  }
  return fixedNodeIndices;
}
}  // namespace

Mesh::PolygonalMesh Mesh::readMeshFile(
    const std::filesystem::path& infilePath) {
  Utility::throwExceptionIfFalse(
      std::filesystem::exists(infilePath),
      "Did not find input file " + infilePath.string() + ".");
  try {
    std::ifstream infile(infilePath);
    readPolygonalMeshHeader(infile);
    const auto nodes = readMeshNodes(infile);
    const auto polygons = readMeshPolygons(infile);
    const auto fixedNodeIndices = readFixedNodeIndices(infile);
    return Mesh::PolygonalMesh(nodes, polygons, fixedNodeIndices);
  } catch (const std::exception& e) {
    Utility::throwException(e.what());
  }
}

void Mesh::computeMeanRatioQualityNumberOfPolygons(
    const std::vector<Mathematics::Polygon>& polygons,
    const std::vector<Mathematics::Vector2D>& nodes,
    std::vector<double>& meanRatioQualityNumbers) {
  Utility::throwExceptionIfFalse(
      polygons.size() == meanRatioQualityNumbers.size(),
      "Mean ratio quality numbers vector size has to match number of "
      "polygons.");

  const auto computeMeanRatioOfPolygon =
      [&nodes](const Mathematics::Polygon& polygon) {
        return Mathematics::getMeanRatio(polygon, nodes);
      };
  std::transform(std::execution::par_unseq, polygons.begin(), polygons.end(),
                 meanRatioQualityNumbers.begin(), computeMeanRatioOfPolygon);
}

std::vector<double> Mesh::computeMeanRatioQualityNumberOfPolygons(
    const std::vector<Mathematics::Polygon>& polygons,
    const std::vector<Mathematics::Vector2D>& nodes) {
  std::vector<double> meanRatioNumbers(polygons.size(), -1.0);
  computeMeanRatioQualityNumberOfPolygons(polygons, nodes, meanRatioNumbers);
  return meanRatioNumbers;
}

bool Mesh::areEqual(const PolygonalMesh& first,
                    const PolygonalMesh& second,
                    const double nodesEqualTolerance) {
  return Mathematics::areEqual(first.getNodes(), second.getNodes(),
                               nodesEqualTolerance)
         && (first.getPolygons() == second.getPolygons())
         && (first.getFixedNodeIndices() == second.getFixedNodeIndices());
}

Mesh::PolygonalMesh Mesh::distortNodesLocally(
    const PolygonalMesh& mesh,
    const double maxDistortionRadius) {
  auto newNodes = mesh.getNodes();
  for (const std::size_t nodeIndex : mesh.getNonFixedNodeIndices()) {
    newNodes.at(nodeIndex) += Mathematics::getRandomVector(maxDistortionRadius);
  }
  return PolygonalMesh(newNodes, mesh.getPolygons(),
                       mesh.getFixedNodeIndices());
}
