function mesh = loadmesh(filename)
  % function mesh = loadmesh(filename)
  %
  % Load mesh from file with given filename.

  mesh = [];

  fid = fopen(filename,'r');
  if fid < 0
    error(['Cannot open file ' filename '.'])
  end
  readheader(fid);
  while ~feof(fid)
    fscanf(fid,'%[ \t\n]');
    if feof(fid)
      break
    end
    keyword = fscanf(fid,'%[a-z_]');
    numberofentries = fscanf(fid,'%d',1);
    switch keyword
      case 'nodes'
        mesh.nodes = readnodes(fid,numberofentries);
      case 'polygons'
        [mesh.polygons,numberofnodesperpolygon] = readpolygons(fid,numberofentries);
      case 'polygon_mean_ratio_quality_numbers'
        if length(numberofnodesperpolygon)~=numberofentries
          error('number of polygons must match number of mean ratio entries');
        end
        mesh.meanratio = readmeanratio(fid,numberofnodesperpolygon);
      case 'fixed_node_indices'
        mesh.fixednodeindices = readfixednodeindices(fid,numberofentries);
      case [' ', '', []]
        % ignore white space entry
      otherwise
        error(['Unrecognized keyword "' keyword '"'])
    end
  end
end  

function readheader(fid)
  line = fgetl(fid);
  if isempty(strfind(line,'planar_polygonal_mesh'))
    error(['file "' filename '" does not contain a planar polygonal mesh']);
  end
end

function nodes = readnodes(fid,numberofnodes)
  nodes = fscanf(fid,'%g',2*numberofnodes);
  if length(nodes)~=2*numberofnodes
    error('could not read node coordinates')
  end
  nodes = [nodes(1:2:end),nodes(2:2:end)];
  fgetl(fid);
end

function [polygons,numofnodesperpolygon] = readpolygons(fid,numberofpolygons)
  rawdata = fscanf(fid,'%d')';
  % determine number of nodes per polyon and data start indices
  numofnodesperpolygon = zeros(numberofpolygons,1);
  polygonnodesstartindices = zeros(numberofpolygons,1);
  index = 1;
  for polygonindex = 1:numberofpolygons
    numofnodes = rawdata(index);
    numofnodesperpolygon(polygonindex) = numofnodes;
    polygonnodesstartindices(polygonindex) = index+1;
    index = index + 1 + numofnodes;
  end
  % collect all polygons with same number of nodes and assign to polygons entry
  polygons = cell(1,max(numofnodesperpolygon));
  for numofnodes = unique(numofnodesperpolygon)'
    relevantstartindices = polygonnodesstartindices( numofnodesperpolygon == numofnodes );
    indexmatrix = repmat(relevantstartindices(:),1,numofnodes) ...
      + repmat(0:numofnodes-1,length(relevantstartindices),1);
    polygons{numofnodes} = 1 + rawdata(indexmatrix);
  end
end

function meanratio = readmeanratio(fid,numberofnodesperpolygon)
  meanratio =  cell(1,max(numberofnodesperpolygon));
  mr = fscanf(fid,'%g',length(numberofnodesperpolygon));
  if length(mr) ~= length(numberofnodesperpolygon)
    error('could not read mean ratio values')
  end
  for numofnodes = unique(numberofnodesperpolygon)'
    meanratio{numofnodes} = mr(numberofnodesperpolygon==numofnodes);
  end
end

function fixednodeindices = readfixednodeindices(fid,numberofindices)
  fixednodeindices = 1 + fscanf(fid,'%d',numberofindices);
  if length(fixednodeindices) ~= numberofindices
    error('could not read fixed node indices')
  end
end
  