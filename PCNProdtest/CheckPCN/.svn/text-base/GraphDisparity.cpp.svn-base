#include "GraphDisparity.h"

GraphDisparity::GraphDisparity(void)
{
}

GraphDisparity::~GraphDisparity(void)
{
}

PixelType* GraphDisparity::printNodes(){
/*
  for ( int i=0; i<this->nodes.size(); ++i ){
  printf("*****************\nNode %d\n", i);
  printf("Value: %d\n", nodes[i].value);
  printf("Label: %d\n", nodes[i].label);
  printf("N pixel: %d\n", nodes[i].positions.size());
  printf("Vicini (index, peso): ");
  for ( int j=0; j<nodes[i].neighbors.size(); ++j )
  printf("(%d, %d), ", nodes[i].neighbors.at(j).first, nodes[i].neighbors.at(j).second);
  printf("\n");
  //printf("N vicini: %d\n", nodes[i].neighbors.size());
  printf("\n");
  }		 
*/
  PixelType *img_out = (PixelType *) calloc(CROP, sizeof(PixelType));
  for( int i=0; i<this->nodes.size(); ++i ){
    for( int j=0; j<this->nodes.at(i).positions.size(); ++j ){
      //if ( i != 2 )
      img_out[nodes.at(i).positions.at(j)] = i*255/nodes.size();
      //else
      //img_out[nodes.at(i).positions.at(j)] = 255;
    }
  }
  /*img_out[6330] = 255;
  img_out[10744] = 255;
  img_out[10380] = 255;*/
  return img_out;
}

void GraphDisparity::addPixel(PixelType value, int label, const int& pos){

  assert( pos>=0 && pos<CROP );
  int index = this->indexNode( label );
  if ( index == -1 )
    this->addNode( value, label, pos );
  else
    this->updateNode(index, pos);
}

int GraphDisparity::indexNode(int label){
  for( int i=0; i<this->nodes.size(); ++i )
    if ( nodes.at(i).label == label )
      return i;
  return -1;
}

void GraphDisparity::addNode( PixelType value, int label, const int& pos ){
  tNode newNode;
  newNode.value = value;
  newNode.label = label;
  newNode.positions.push_back( pos );
  nodes.push_back( newNode );
}

void GraphDisparity::updateNode( const int& index, const int& pos ){
  this->nodes.at(index).positions.push_back( pos );
}

int GraphDisparity::getNNodes(){
  return this->nodes.size();
}

void GraphDisparity::calcNear(){

  // Flags delle posizioni visitate/non visitate
  bool* img_flags = (bool*) calloc( W_CROP*H_CROP, sizeof(bool) );

  vector<int> positions;
  for( int i=0; i<nodes.size(); ++i ){

    for( int j=0; j<nodes.at(i).positions.size(); ++j ){
      this->getNeighborhood( nodes.at(i).positions.at(j), positions, img_flags );
      for( int k=0; k<positions.size(); ++k ){
        updateNearPositions(i, positions.at(k) );
      }
    }
  }

}

void GraphDisparity::getNeighborhood(const int& pos, vector<int>& positions, bool* img_flags){

  int n_near_positions = 8;
  positions = vector<int>(n_near_positions, -1);

	vector<int> near_(n_near_positions,0);
	near_[0] = pos-1;
	near_[1] = pos+1;
	near_[2] = pos-1+W_CROP;
	near_[3] = pos+1+W_CROP;
	near_[4] = pos+W_CROP;
	near_[5] = pos-1-W_CROP;
	near_[6] = pos+1-W_CROP;
	near_[7] = pos-W_CROP;

	for( int i=0; i<near_.size(); ++i ){
		if ( near_[i] >=0 && near_[i] <CROP && !img_flags[near_[i]] && isCoordNear(pos, near_[i]) ){
			positions.at(i) = near_[i];
			img_flags[near_[i]] = true;
			}else
				positions.at(i) = -1;
		}
  /*std::pair<int, int> coord = getCoordToPos( pos );
  std::pair<int, int> coorda = getCoordToPos( a );
  std::pair<int, int> coordb = getCoordToPos( b );
  std::pair<int, int> coordc = getCoordToPos( c );
  std::pair<int, int> coordd = getCoordToPos( d );
  std::pair<int, int> coorde = getCoordToPos( e );
  std::pair<int, int> coordf = getCoordToPos( f );
  std::pair<int, int> coordg = getCoordToPos( g );
  std::pair<int, int> coordh = getCoordToPos( h );
  printf("***********************\n");
  printf("(%d, %d)\n", coord.first, coord.second);
  printf("(%d, %d)\n", coorda.first, coorda.second);
  printf("(%d, %d)\n", coordb.first, coordb.second);
  printf("(%d, %d)\n", coordc.first, coordc.second);
  printf("(%d, %d)\n", coordd.first, coordd.second);
  printf("(%d, %d)\n", coorde.first, coorde.second);
  printf("(%d, %d)\n", coordf.first, coordf.second);
  printf("(%d, %d)\n", coordg.first, coordg.second);
  printf("(%d, %d)\n", coordh.first, coordh.second);*/

  /*if ( a >=0 && a <CROP && isCoordNear(pos,a) && !img_flags[a]){
    positions.at(0) = a;
    img_flags[a] = true;
  }else
    positions.at(0) = -1;

  if ( b >=0 && b <CROP && isCoordNear(pos,b) && !img_flags[b]){
    positions.at(1) = b;
    img_flags[b] = true;
  }else
    positions.at(1) = -1;

  if ( c >=0 && c <CROP && isCoordNear(pos,c) && !img_flags[c]){
    positions.at(2) = c;
    img_flags[c] = true;
  }else
    positions.at(2) = -1;

  if ( d >=0 && d <CROP && isCoordNear(pos,d) && !img_flags[d]){
    positions.at(3) = d;
    img_flags[d] = true;
  }else
    positions.at(3) = -1;

  if ( e >=0 && e <CROP && isCoordNear(pos,e) && !img_flags[e]){
    positions.at(4) = e;
    img_flags[e] = true;
  }else
    positions.at(4) = -1;

  if ( f >=0 && f <CROP && isCoordNear(pos,f) && !img_flags[f]){
    positions.at(5) = f;
    img_flags[f] = true;
  }else
    positions.at(5) = -1;

  if ( g >=0 && g <CROP && isCoordNear(pos,g) && !img_flags[g]){
    positions.at(6) = g;
    img_flags[g] = true;
  }else
    positions.at(6) = -1;

  if ( h >=0 && h <CROP && isCoordNear(pos,h) && !img_flags[h]){
    positions.at(7) = h;
    img_flags[h] = true;
  }else
    positions.at(7) = -1;*/
}

void GraphDisparity::updateNearPositions(const int& index, const int& near_pos){
  if ( near_pos == -1 )
    return;

  int index_near = findNodeFromPos( near_pos );
  if ( index != index_near )
    // La posizione vicina non è anch'essa nel nodo index
    if ( !this->isNear( index, index_near )){	//!< Controllo se il collegamento è già presente
      //std::pair<int, int> coord = getCoordToPos( near_pos );
      //printf("%d %d (%d, %d)\n", index, index_near, coord.first, coord.second);
      int arcWeight = (nodes.at(index_near).value - nodes.at(index).value)/16;
      this->nodes.at(index).neighbors.push_back(std::make_pair(index_near,arcWeight));
      this->nodes.at(index_near).neighbors.push_back(std::make_pair(index,-arcWeight));
    }

}

int GraphDisparity::findNodeFromPos( const int& pos ){
  for( int i=0; i<nodes.size(); ++i )
    for( int j=0; j<nodes.at(i).positions.size(); ++j )
      if ( pos == nodes.at(i).positions.at(j) )
        return i;
  return -1;
}

bool GraphDisparity::isNear( const int& index, const int& near_index ){

  for( int j=0; j<nodes.at(index).neighbors.size(); ++j )
    if ( nodes.at(index).neighbors.at(j).first == near_index )
      return true;
  return false;

}

std::pair<int, int> GraphDisparity::getCoordToPos( const int& pos ){
  return std::make_pair(pos/W_CROP, pos%W_CROP);
}

bool GraphDisparity::isCoordNear(const int& first, const int& second){
  std::pair<int, int> f = getCoordToPos(first), s = getCoordToPos(second);
  if ( abs(f.first-s.first) <= 1 && abs(f.second-s.second) <= 1 )
    return true;
  return false;
}



tResultsCheckGraph GraphDisparity::checkBorderDisparity(){
	tResultsCheckGraph out;
	out.name = "Check border disparity";
  //if ( this->bordersEnd.size() == 0 || this->bordersFirst.size() == 0)
   // return false;
	int disparity = 0;
	bool response = true;
  if ( this->bordersFirst.size() > 0)
  {
    disparity = nodes[bordersFirst.at(0)].value;
    for ( int i=0; i<bordersFirst.size(); ++i )
      if ( disparity != nodes[bordersFirst.at(i)].value )
				{
				addPosToVect(out.positions, nodes[bordersFirst.at(i)].positions);
				response = false;
				}
  }
  if (this->bordersEnd.size() > 0)
  {
    disparity = nodes[bordersEnd.at(0)].value;
    for ( int i=0; i<bordersEnd.size(); ++i )
      if ( disparity != nodes[bordersEnd.at(i)].value )
				{
				addPosToVect(out.positions, nodes[bordersEnd.at(i)].positions);
				response = false;
				}
  }
	out.response = response;
  return out;
}

tResultsCheckGraph GraphDisparity::checkInternalBlob(){
	tResultsCheckGraph out;
	out.name = "Check internal blob";
  bordersFirst.clear();
  bordersEnd.clear();
  for ( int i=0; i<nodes.size(); ++i )
    if (nodes.at(i).neighbors.size() == 1){
      bool internalBlob(true);
      for( int j=0; j<nodes.at(i).positions.size(); ++j ){
        int y = this->getCoordToPos(nodes.at(i).positions.at(j)).first;
        if ( y == 0 ){
          //printf("Estremo superiore: %d\n", i);
          bordersFirst.push_back( i );
          internalBlob = false;
          break;
        }else
          if ( y == H_CROP-1 ){
            //printf("Estremo inferiore: %d\n", i);
            bordersEnd.push_back( i );
            internalBlob = false;
            break;
						}
      }
      if ( internalBlob )
				{
				addPosToVect(out.positions, nodes.at(i).positions);
				out.response = false;
				return out;
				}
    }
		out.response = true;
    return out;
}

void GraphDisparity::nodePruning()
{

  int num_nodes = this->nodes.size();
  int limit = 0;
  for (int i = 0; i < num_nodes; ++i)
  {
    int y_c = 0, x_c = 0;
    computeCentroid(nodes[i],y_c,x_c);

    if( y_c <= SIZE_BORDER_CHECK || y_c >= H_CROP-SIZE_BORDER_CHECK ||
      x_c <= SIZE_BORDER_CHECK || x_c >= W_CROP-SIZE_BORDER_CHECK)
      limit = MIN_NUM_PIXEL_PER_NODE_BORDER;
    else
      limit = MIN_NUM_PIXEL_PER_NODE;

    if ( nodes[i].positions.size() < limit)
    {
      nodes.erase(nodes.begin()+i);
      pruningEdge(i);
      i--; num_nodes--;
    }

  }
  return;
}

void GraphDisparity::pruningEdge(const int& index_nodes)
{
  /*int num_nodes = this->nodes.size();
  for (int i = 0; i < num_nodes; ++i)
  {
  std::vector<std::pair<int,int>> neighbors_tmp = nodes[i].neighbors;
  int num_edge = neighbors_tmp.size();
  for ( int e = 0; e < num_edge; ++e)
  {
  std::pair<int,int> edge_tmp = neighbors_tmp.at(e);
  if ( edge_tmp.first == index_nodes)
  {
  nodes[i].neighbors.erase(nodes[i].neighbors.begin()+e);
  e--; num_edge--;
  }else
  if ( edge_tmp.first > index_nodes )
  neighbors_tmp.at(e).first--;

  }
  }*/
  for( int i=0; i<nodes.size(); ++i ){
    int size_arcs = nodes.at(i).neighbors.size();
    for( int j=0; j<size_arcs; ++j ){
      if ( nodes.at(i).neighbors.at(j).first == index_nodes ){
        nodes[i].neighbors.erase(nodes[i].neighbors.begin()+j);
        j--; size_arcs--;
      }else{
        if ( nodes.at(i).neighbors.at(j).first > index_nodes ){
          nodes.at(i).neighbors.at(j).first--;
        }
      }
    }
  }

}

tResultsCheckGraph GraphDisparity::checkWeightEdge()
{
	tResultsCheckGraph out;
  bool ret = true;
  int num_nodes = this->nodes.size();
  for (int i = 0; i < num_nodes; ++i)
  {
    for ( int e = 0; e < nodes[i].neighbors.size(); ++e)
    {
      std::pair<int,int> tmp = nodes[i].neighbors.at(e);

			bool temp_ret(false);
			if (tmp.second == -1 || tmp.second == 1){
				temp_ret = true;
				}else{
					temp_ret = false;
					addPosToVect(out.positions, nodes.at(i).positions);
					addPosToVect(out.positions, nodes.at(tmp.first).positions);
				}
      ret = ret && temp_ret;
    }
  }
	out.response = ret;
	out.name = "Check weight edges";
  return out;
}

void GraphDisparity::addPosToVect(vector<int>& out, const vector<int>& in){
	for( int i=0; i<in.size(); ++i ){
		out.push_back(in.at(i));
		}
	}

bool GraphDisparity::isNotBorderNodes( const int& index ){
  for ( int i=0; i<this->bordersFirst.size(); ++i )
    if ( bordersFirst.at(i) == index )
      return false;
  for (	int j=0; j<this->bordersEnd.size(); ++j)
    if ( bordersEnd.at(j) == index )
      return false;
  return true;
}

tResultsCheckGraph GraphDisparity::checkCentroid()
{
	tResultsCheckGraph out;
	out.name = "Check centroids alignment";
  bool ret = true;
  int num_nodes = this->nodes.size();
  for (int i = 0; i < num_nodes; ++i)
  {
    //if ( !this->isNotBorderNodes(i) )
    //  continue;
    int y = 0;
    int x = 0;
    computeCentroid(this->nodes.at(i),y,x);
		
    if (isNotBorderNodes(i))// || nodes.at(i).positions.size() > MIN_NUM_PIXEL_FOR_CENTROID_CHECK)
    {
			out.positions.push_back(y*W_CROP + x);
      //printf(" Nodo: %d, X : %d, Y: %d, num_pixel: %d \n",i,x,y,nodes.at(i).positions.size());
      ret = ret && ( x > (W_CROP/2)*0.75 && x < (W_CROP/2)*1.75) ? true : false;
      //printf(" Nodo: %d, X : %d, Y: %d, num_pixel: %d, res : %d \n",i,x,y,nodes.at(i).positions.size(),( x > (W_CROP/2)*0.75 && x < (W_CROP/2)*1.75));
    }
  }

	out.response = ret;
  return out;
}

void GraphDisparity::computeCentroid(tNode& node, int& y_c,  int& x_c)
{

  int num_pixel = node.positions.size();
  for ( int i = 0; i < num_pixel; ++i)
  {
    std::pair<int,int> coord = getCoordToPos(node.positions.at(i));
    y_c += coord.first;
    x_c += coord.second;
  }

  y_c = (int) y_c/num_pixel;
  x_c = (int) x_c/num_pixel;

  return;
}

tResultsCheckGraph GraphDisparity::checkPath()
{
tResultsCheckGraph out;
out.name = "Check path";
  bool ret = false;
  int num_nodes = this->nodes.size();
  for (int i = 0; i < num_nodes; ++i)
  {
    if(isNodeBorderFirst(i))
      ret = ret || check_path(i);
  }

	out.response = ret;
  return out;
}

bool GraphDisparity::check_path( int& index )
{
    bool ret(false);
  if (isNodeBorderEnd(index))
    ret = true;
  else
  {
    std::vector< std::pair<int,int> > neighbors_tmp = nodes[index].neighbors;
    int num_edge = neighbors_tmp.size();
    for ( int e = 0; e < num_edge; ++e)
    {
      std::pair<int,int> edge_tmp = neighbors_tmp.at(e);
      if ( edge_tmp.second == 1)
        ret = ret || check_path(edge_tmp.first);
    }
  }
  return ret;
}
bool GraphDisparity::isNodeBorderFirst( const int& index )
{
  for ( int i=0; i<this->bordersFirst.size(); ++i )
    if ( bordersFirst.at(i) == index )
      return true;

  return false;
}
bool GraphDisparity::isNodeBorderEnd( const int& index )
{
  for ( int i=0; i<this->bordersEnd.size(); ++i )
    if ( bordersEnd.at(i) == index )
      return true;

  return false;
}

tResultsCheckGraph GraphDisparity::checkNumDisparity()
{
	tResultsCheckGraph out;
	out.name = "Check number disparity";
  bool ret = true;
  int num_nodes = this->nodes.size();
  std::vector<int> disparity;
  for (int i = 0; i < num_nodes; ++i)
  {
    if (!justFound(nodes[i].value,disparity))
    {
      disparity.push_back(nodes[i].value);
      //printf(" disparita[%d] : %d",i,nodes[i].value);
    }
  }

  if ( disparity.size() < 3 || disparity.size() > 8)
    ret = false;

	out.response = ret;
  return out;
}

bool GraphDisparity::justFound(const int& dsp_value,vector<int> & disparity)
{
  bool ret = false;
  int sz_dsp = disparity.size();
  if ( sz_dsp == 0)
  {
    disparity.push_back(dsp_value);
    ret = false;
  }
  else
  {
    for ( int i = 0; i < sz_dsp;++i)
    {
      if ( dsp_value == disparity.at(i))
        ret =  true;
    }
  }

  return ret;
}

tResultsCheckGraph GraphDisparity::checkShape()
{
tResultsCheckGraph out;
out.name = "Check shape";
  bool ret = true;
  int num_nodes = this->nodes.size();
  for (int i = 0; i < num_nodes; ++i)
  {
    if (nodes.at(i).positions.size() < MIN_NUM_PIXEL_FOR_CENTROID_CHECK)
    {
      int y_l_min = H_CROP;
      int y_r_min =H_CROP;
      int y_l_max = 0;
      int y_r_max = 0;
      int x_l = W_CROP;
      int x_r = 0;
      findlimits(this->nodes.at(i),x_l,y_l_max,y_l_min,x_r,y_r_max,y_r_min);
      //printf(" nodo %d, x_min : %d,y_l_min: %d,y_l_max : %d \n",nodes[i].value,x_l,y_l_min,y_l_max);
      //printf(" nodo %d, x_max : %d,y_r_min: %d,y_r_max : %d \n",nodes[i].value,x_r ,y_r_min,y_r_max);
      // Controllo i limiti
      if (!isNotBorderNodes(i))
        ret = ret && ((isNodeBorderFirst(i) && abs(y_l_max-y_r_max) <= MAX_ROW_DELTA) || (isNodeBorderEnd(i) && abs(y_l_min-y_r_min) <= MAX_ROW_DELTA)) ? true : false;
      else
        ret = ret && (abs(y_l_min-y_r_min) <= MAX_ROW_DELTA && abs(y_l_max-y_r_max) <= MAX_ROW_DELTA) ? true : false;
    }
  }

	out.response = ret;
  return out;
}

void GraphDisparity::findlimits(tNode& node,
                                int& x_l,
                                int& y_l_max,
                                int& y_l_min,
                                int& x_r,
                                int& y_r_max,
                                int& y_r_min)
{
  int num_pixel = node.positions.size();
  for ( int i = 0; i < num_pixel; ++i)
  {
    std::pair<int,int> coord = getCoordToPos(node.positions.at(i));
    x_l = min(coord.second,x_l);
    x_r = max(coord.second,x_r);
  }
  for ( int i = 0; i < num_pixel; ++i)
  {
    std::pair<int,int> coord = getCoordToPos(node.positions.at(i));
    y_l_min = (coord.second == x_l) ? min(coord.first, y_l_min) : y_l_min;
    y_l_max = (coord.second == x_l) ? max(coord.first, y_l_max) : y_l_max;
    y_r_min = (coord.second == x_r) ? min(coord.first, y_r_min) : y_r_min;
    y_r_max = (coord.second == x_r) ? max(coord.first, y_r_max) : y_r_max;
  }


  return;
}

void GraphDisparity::clearForCheck(){
  // Clear near
  nodes.clear();
  bordersFirst.clear();
  bordersEnd.clear();
}

vector<tResultsCheckGraph> GraphDisparity::check(PixelType* img, const int& w, const int& h){
  this->clearForCheck();

  int size = h*w;
  PixelType *out_uc = (PixelType *) malloc(size*sizeof(PixelType));

  ConnectedComponents cc(50);
  cc.connected(img, out_uc, w, h, std::equal_to<unsigned char>(), true);

  for(int i=0; i<size; ++i )
    addPixel(img[i], out_uc[i], i);

  return checkGraph();
}

PixelType* GraphDisparity::printResults(vector<tResultsCheckGraph> res){

	PixelType* image = (PixelType*) calloc(CROP, sizeof(PixelType));

	int count_failure = 0;
	for( int i=0; i<res.size(); ++i )
			count_failure += !res.at(i).response?1:0;

	for( int i=0; i<res.size(); ++i )
		{
        //printf((char*)(res.at(i).name + ": " + (res.at(i).response?"SUCCESS":"FAILURE") + "\n").c_str() );
        qDebug() << QString::fromStdString(res.at(i).name) + ": " + (res.at(i).response?"SUCCESS":"FAILURE");


		for( int j=0; j<res.at(i).positions.size(); ++j )
			{
			//cout << res.at(i).positions.at(j) << endl;
			if ( !res.at(i).response )
				image[res.at(i).positions.at(j)] = (i+1)*(255/count_failure);
			}
		}
	return image;
	}


vector<tResultsCheckGraph> GraphDisparity::checkGraph()
{
  vector<tResultsCheckGraph> out;

  this->calcNear();

  // Pulisco i nodi
  this->nodePruning();

  // Controllo che ogni nodo abbia solo edge di peso uno
  out.push_back( this->checkWeightEdge());

  out.push_back( this->checkInternalBlob());
  //out.push_back(std::make_pair("Check internal blob", internal_blob));

  out.push_back( this->checkBorderDisparity());
  //out.push_back(std::make_pair("Check borders disparity", borders_disparity));

  out.push_back(  this->checkCentroid());
  //out.push_back(std::make_pair("Check alignment centroids", centroid_check));

  out.push_back( this->checkPath() );
  //out.push_back(std::make_pair("Check path", path_check));

  out.push_back( this->checkNumDisparity());
  //out.push_back(std::make_pair("Check number dispairty", dsp_check));

  out.push_back( this->checkShape());
  //out.push_back(std::make_pair("Check shape", shape_check));

  //printResults(out);
  return out;

  //return weight_edge_ceck && internal_blob && borders_disparity && centroid_check && path_check && dsp_check && shape_check;
}

void GraphDisparity::setParameters(tParametersCheck param){
    MIN_NUM_PIXEL_PER_NODE_BORDER = param.min_num_pixel_for_node;
    MIN_NUM_PIXEL_PER_NODE = param.min_num_pixel_for_node;
    MIN_NUM_PIXEL_FOR_CENTROID_CHECK = param.min_num_pixel_for_centroid;
    MAX_ROW_DELTA = param.max_row_delta;
}
