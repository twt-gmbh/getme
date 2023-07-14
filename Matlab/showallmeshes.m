%{
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
%}

function showallmeshes(directory)
  % function showallmeshes
  %
  % Load and show all meshes in the given directory (current if empty)

  if nargin == 0
    directory = pwd;
  end
  filelist = dir(directory);
  for index = 1:length(filelist)
    [~,~,extension] = fileparts(filelist(index).name);
    if strcmp(extension,'.mesh')
      meshfilepath = fullfile(filelist(index).folder,filelist(index).name);
      mesh = loadmesh(meshfilepath);
      showmesh(mesh);
      title(filelist(index).name,'Interpreter','none')
    end
  end
end