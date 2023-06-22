function config = showmesh(mesh,config) 
  % function config = showmesh(mesh,config) 
  % 
  % Show mesh and return visualization configuration
  %
  % Syntax:
  %   showmesh(mesh);
  %   showmesh(mesh,config);
  %   config = showmesh();
  %
  % Description:
  %   showmesh(mesh) show given mesh
  %   showmesh(mesh,config) show mesh using given configuration
  %   config = showmesh() get default configuration

  if ~exist('config','var')
    config = [];
    config.shownodes = false;
    config.nodesmarker = '.';
    config.nodesmarkersize = 20;
    config.nonfixednodescolor = [0,0,0];
    config.fixednodescolor = [1,0,0];
    config.shownodenumber = false;
    config.nodenumberfontsize = 20;
    config.nodenumbercolor = [0,0,0];
    config.colorelementsbymeanratio = true;
    config.showmeanratiocolorbar = true;
    config.colorbarlocation = 'EastOutside';
    config.fontsize = 15;
    config.newfigure = true;
    if ~exist('mesh','var')
      return;
    end
  end

  if config.newfigure
    figure
  end
  set(gca,'fontsize',config.fontsize)
  hold on
  x = mesh.nodes(:,1);
  y = mesh.nodes(:,2);
  z = 0 * x;

  if config.colorelementsbymeanratio && isfield(mesh,'meanratio')
    % Polygons colored by quality
    for index = 1:length(mesh.polygons)
      if ~isempty(mesh.polygons{index})
        nodeindices = mesh.polygons{index}';
        mrvec = mesh.meanratio{index};
        mrvec(mrvec<0) = 0;
        patch(x(nodeindices),y(nodeindices),z(nodeindices), mrvec);
      end
    end
    mycolormap = brighten(flipud(jet),0.5);
    colormap(mycolormap);
    caxis([0,1]);
    if config.showmeanratiocolorbar 
      colorbar(config.colorbarlocation);
    end
    title('Polygons colored by mean ratio quality')
  else
    % Polygons wireframe
    for index = 1:length(mesh.polygons)
      if ~isempty(mesh.polygons{index})
        nodeindices = mesh.polygons{index}';
        if all(size(nodeindices)>1)
          nodeindices = nodeindices([1:end,1],:);
        else
          nodeindices = [nodeindices(:);nodeindices(1)]';
        end
        plot(x(nodeindices),y(nodeindices),'k');
      end
    end
  end

  % Nodes
  if config.shownodes
    isnodefixed = false(1,length(x));
    if isfield(mesh,'fixednodeindices')
      isnodefixed(mesh.fixednodeindices) = true;
    end
    plot(x(isnodefixed),y(isnodefixed),config.nodesmarker,'markersize',config.nodesmarkersize,'color',config.fixednodescolor);
    plot(x(~isnodefixed),y(~isnodefixed),config.nodesmarker,'markersize',config.nodesmarkersize,'color',config.nonfixednodescolor);
  end

  % Node numbers
  if config.shownodenumber
    for index = 1:length(x)
      text(x(index),y(index),num2str(index-1),...
          'color',config.nodenumbercolor,...
          'fontsize',config.nodenumberfontsize,...
          'VerticalAlignment','top',...
          'HorizontalAlignment','left');
    end
  end

  axis equal
end
